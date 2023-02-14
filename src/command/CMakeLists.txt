add_library(beatled_commands INTERFACE)

target_sources(
  beatled_commands INTERFACE 
    command.c
    hello/hello.c
    time/time.c
    tempo/tempo.c
    next_beat/next_beat.c
)

target_include_directories(beatled_commands INTERFACE ${CMAKE_CURRENT_LIST_DIR})
