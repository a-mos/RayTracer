cmake_minimum_required(VERSION 3.8)
project(rt)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -fopenmp")

include_directories(.)

add_executable(rt main.cpp)
