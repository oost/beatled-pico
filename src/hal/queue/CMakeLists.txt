set(CURRENT_LIBRARY_NAME beatled_hal_queue)

add_library(${CURRENT_LIBRARY_NAME})

target_include_directories(${CURRENT_LIBRARY_NAME}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(ports/${PORT})
