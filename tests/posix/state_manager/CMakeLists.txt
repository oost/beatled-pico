add_executable(test_state_manager
  test_state_manager.cpp
  state_stubs.c
  ${CMAKE_SOURCE_DIR}/src/state_manager/state_manager.c
)
target_include_directories(test_state_manager PRIVATE
  ${CMAKE_SOURCE_DIR}/src/state_manager/include
)
target_link_libraries(test_state_manager PRIVATE
  Catch2::Catch2WithMain
  beatled_event
  beatled_hal_board
  beatled_hal_time
)
if(NOT VCPKG_TARGET_TRIPLET)
  catch_discover_tests(test_state_manager)
endif()
