add_library(beatled_state_manager INTERFACE)

target_sources(
    beatled_state_manager INTERFACE 
        state_manager.c
        states/started/started.c
        states/initialized/initialized.c
        states/registered/registered.c
        states/time_synced/time_synced.c
)

target_link_libraries(beatled_state_manager INTERFACE
        # hardware_timer
        # hardware_clocks
        # pico_multicore
        beatled_hal_wifi
        )