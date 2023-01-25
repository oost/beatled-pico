
if(PORT STREQUAL "pico")
  set(PICO_PORT ON) 
  set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/pico-sdk")
  include(pico_sdk_import.cmake)
elseif(PORT STREQUAL "posix")
  SET(POSIX_PORT ON)
else()
  message(FATAL_ERROR "Need to specify port. Got PORT='${PORT}'")
endif()

message("Building ${PORT} port")


macro(port_init)
  if (PICO_PORT)
    message("Initializing PICO SDK")
    pico_sdk_init()
    
    #  ????? 
    add_compile_definitions(PICO_PORT)

  endif()

  if(POSIX_PORT)
    message("Initializing POSIX configurations")

    add_compile_definitions(POSIX_PORT)

    if(STDC_NO_THREADS)
      message( FATAL_ERROR "Need thread support" )
    else()
      message("Threads are supported.")
    endif()

  endif()
endmacro()