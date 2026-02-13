add_executable(test_command test_command.cpp)
target_link_libraries(test_command PRIVATE
  Catch2::Catch2WithMain
  beatled_commands
  beatled_ws2812
)
if(NOT VCPKG_TARGET_TRIPLET)
  catch_discover_tests(test_command)
endif()
