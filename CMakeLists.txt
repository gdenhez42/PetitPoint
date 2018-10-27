cmake_minimum_required (VERSION 3.12.3)
project (PetitPoint)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

link_directories(external)

include_directories(include
                    external)

file(GLOB SOURCES "src/*.cpp")

add_executable(PetitPoint ${SOURCES})

target_link_libraries(PetitPoint tinyxml2
                                 SDL2
                                 SDL2_image
                                 SDL2_ttf
                                 SDL2_mixer)
