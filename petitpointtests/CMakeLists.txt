cmake_minimum_required (VERSION 3.12.3)
project (PetitPoint)

link_directories(../external/googletest/lib)

include_directories(../external/googletest/include)

file(GLOB SOURCES "*.cpp")

add_executable(PetitPointTest ${SOURCES})

target_link_libraries(PetitPointTest gtest
                                     gtest_main
                                     pthread)