add_executable(test_event_queue test_event_queue.cpp)
target_include_directories(test_event_queue PRIVATE
  ${CMAKE_SOURCE_DIR}/src/state_manager/include
)
target_link_libraries(test_event_queue PRIVATE
  Catch2::Catch2WithMain
  beatled_event
  beatled_hal_time
  beatled_hal_board
)
if(NOT VCPKG_TARGET_TRIPLET)
  catch_discover_tests(test_event_queue)
endif()
