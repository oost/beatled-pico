add_library(beatled_commands INTERFACE)

target_sources(
  beatled_commands INTERFACE 
    command.c
    hello/hello.c
    time/time.c
    tempo/tempo.c
)