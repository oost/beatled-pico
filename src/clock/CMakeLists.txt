add_library(clock INTERFACE clock.c)

target_include_directories(clock INTERFACE ${CMAKE_CURRENT_LIST_DIR})
