cmake_minimum_required (VERSION 3.10.2)
project (PetitPoint)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory (petitpoint)
add_subdirectory (petitpointtests)
