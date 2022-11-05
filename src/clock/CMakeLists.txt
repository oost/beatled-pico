add_library(beatled_clock INTERFACE )

target_sources(
  beatled_clock INTERFACE 
        clock.c
)

target_include_directories(beatled_clock INTERFACE ${CMAKE_CURRENT_LIST_DIR})
