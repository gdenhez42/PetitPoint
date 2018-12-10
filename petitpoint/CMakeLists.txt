cmake_minimum_required (VERSION 3.12.3)
project (PetitPoint)

link_directories(../external/tinyxml2)

include_directories(include
                    ../external/tinyxml2)

file(GLOB SOURCES "src/*.cpp")

add_executable(PetitPoint ${SOURCES})

target_link_libraries(PetitPoint tinyxml2
                                 SDL2
                                 SDL2_image
                                 SDL2_ttf
                                 SDL2_mixer)
