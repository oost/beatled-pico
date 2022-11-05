target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    queue.c
    circular_buffer.c
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(beatled_hal_queue
  Threads::Threads
)
