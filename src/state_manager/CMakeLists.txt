add_library(state_manager INTERFACE)

target_sources(
    state_manager INTERFACE 
        state_manager.c
        states/started/started.c
        states/initialized/initialized.c
        states/registered/registered.c
        states/time_synced/time_synced.c
)