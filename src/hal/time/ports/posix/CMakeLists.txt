target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    time.c
    alarm.c
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CURRENT_LIBRARY_NAME}
  Threads::Threads
)
