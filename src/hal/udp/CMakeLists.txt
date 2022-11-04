# add_library(beatled_hal_udp 
#     udp.c
# )

# target_link_libraries(beatled_hal_udp
#   pico_stdlib
#   pico_cyw43_arch_lwip_threadsafe_background
# )



set(CURRENT_LIBRARY_NAME beatled_hal_udp)

add_library(${CURRENT_LIBRARY_NAME})

target_include_directories(${CURRENT_LIBRARY_NAME}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(ports/${PORT})

target_include_directories(${CURRENT_LIBRARY_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/..
)
