add_library(commands INTERFACE)

target_sources(
  commands INTERFACE 
    command.c
    hello/hello.c
    time/time.c
    tempo/tempo.c
)