#include <catch2/catch_test_macros.hpp>

#include "hal/queue.h"
#include <cstdint>

typedef struct msg {
  int id;
} msg_t;

hal_queue_handle_t queue_ptr;

TEST_CASE("Queues are created", "[queue]") {
  queue_ptr = hal_queue_init(sizeof(msg), 10);
  msg_t msg1 = {.id = 1};
  msg_t msg2 = {.id = 2};

  SECTION("Capacity and size work as expected") {
    REQUIRE(hal_queue_capacity(queue_ptr) == 10);
    REQUIRE(hal_queue_size(queue_ptr) == 0);
    msg_t msg1 = {.id = 1};
    hal_queue_add_message(queue_ptr, &msg1);
    REQUIRE(hal_queue_size(queue_ptr) == 1);
  }

  SECTION("Messages get copied in the queue") {
    REQUIRE(hal_queue_add_message(queue_ptr, &msg1));
    REQUIRE(msg2.id == 2);
    REQUIRE(hal_queue_pop_message(queue_ptr, &msg2));
    REQUIRE(msg2.id == 1);
    REQUIRE(!hal_queue_pop_message(queue_ptr, &msg2));
    REQUIRE(hal_queue_size(queue_ptr) == 0);
    msg2.id = 2;
    REQUIRE(!hal_queue_pop_message(queue_ptr, &msg2));
    REQUIRE(hal_queue_size(queue_ptr) == 0);
    REQUIRE(msg2.id == 2);
  }

  hal_queue_free(queue_ptr);
}