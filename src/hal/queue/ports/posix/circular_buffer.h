/// https://embeddedartistry.com/blog/2017/05/17/creating-a-circular-buffer-in-c-and-c/

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "hal/queue.h"

/// Pass in a storage buffer and size, returns a circular buffer handle
/// Requires: buffer is not NULL, size > 0 (size > 1 for the threadsafe
//  version, because it holds size - 1 elements)
/// Ensures: me has been created and is returned in an empty state
hal_queue_handle_t circular_buf_init(unsigned int element_count,
                                     size_t element_size);

/// Free a circular buffer structure
/// Requires: me is valid and created by circular_buf_init
/// Does not free data buffer; owner is responsible for that
void circular_buf_free(hal_queue_handle_t me);

/// Reset the circular buffer to empty, head == tail. Data not cleared
/// Requires: me is valid and created by circular_buf_init
void circular_buf_reset(hal_queue_handle_t me);

/// Put that continues to add data if the buffer is full
/// Old data is overwritten
/// Note: if you are using the threadsafe version, this API cannot be used,
/// because it modifies the tail pointer in some cases. Use circular_buf_try_put
/// instead. Requires: me is valid and created by circular_buf_init
void circular_buf_put(hal_queue_handle_t me, uint8_t data);

/// Put that rejects new data if the buffer is full
/// Note: if you are using the threadsafe version, *this* is the put you should
/// use Requires: me is valid and created by circular_buf_init Returns 0 on
/// success, -1 if buffer is full
int circular_buf_try_put(hal_queue_handle_t me, uint8_t data);

/// Retrieve a value from the buffer
/// Requires: me is valid and created by circular_buf_init
/// Returns 0 on success, -1 if the buffer is empty
int circular_buf_get(hal_queue_handle_t me, uint8_t *data);

/// CHecks if the buffer is empty
/// Requires: me is valid and created by circular_buf_init
/// Returns true if the buffer is empty
bool circular_buf_empty(hal_queue_handle_t me);

/// Checks if the buffer is full
/// Requires: me is valid and created by circular_buf_init
/// Returns true if the buffer is full
bool circular_buf_full(hal_queue_handle_t me);

/// Check the capacity of the buffer
/// Requires: me is valid and created by circular_buf_init
/// Returns the maximum capacity of the buffer
unsigned int circular_buf_capacity(hal_queue_handle_t me);

/// Check the number of elements stored in the buffer
/// Requires: me is valid and created by circular_buf_init
/// Returns the current number of elements in the buffer
unsigned int circular_buf_size(hal_queue_handle_t me);

/// Look ahead at values stored in the circular buffer without removing the data
/// Requires:
///		- me is valid and created by circular_buf_init
///		- look_ahead_counter is less than or equal to the value returned
/// by circular_buf_size()
/// Returns 0 if successful, -1 if data is not available
int circular_buf_peek(hal_queue_handle_t me, uint8_t *data,
                      unsigned int look_ahead_counter);

// TODO: int circular_buf_get_range(circular_buf_t me, uint8_t *data, size_t
// len);
// TODO: int circular_buf_put_range(circular_buf_t me, uint8_t * data, size_t
// len);

#endif // CIRCULAR_BUFFER_H_