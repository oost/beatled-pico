
# These tests can use the Catch2-provided main
add_executable(test_queue test_queue.cpp)
target_link_libraries(test_queue PRIVATE 
  Catch2::Catch2WithMain
  beatled_hal_queue
)

catch_discover_tests(test_queue)