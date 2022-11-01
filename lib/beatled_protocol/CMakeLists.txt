cmake_minimum_required(VERSION 3.13)

project(beatled_protocol)
set(CMAKE_C_STANDARD 11)
add_library(beatled_protocol INTERFACE)
target_include_directories(beatled_protocol INTERFACE include)
