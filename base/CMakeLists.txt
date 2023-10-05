cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(
  hello_world 
  VERSION 1.0
  LANGUAGES CXX)

#add_library(mylib my_lib.cpp my_lib.hpp)
add_executable(server server.cpp)

add_executable(client client.cpp)

#target_link_libraries(main PRIVATE mylib)

