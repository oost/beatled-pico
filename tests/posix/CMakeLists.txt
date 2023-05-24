if (POSIX_PORT)

  find_package(Catch2 3 REQUIRED)
  include(Catch)

  add_subdirectory(fibonacci)
  add_subdirectory(queue)
  add_subdirectory(clock)
endif()