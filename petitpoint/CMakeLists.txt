cmake_minimum_required (VERSION 3.12.3)
project (PetitPoint)

link_directories(../external/tinyxml2)

if (${WIN32})
	include_directories("e:/vc_dev_libs/SDL2-2.0.9/include"
	                    "e:/vc_dev_libs/SDL2_image-2.0.4/include"
						"e:/vc_dev_libs/SDL2_ttf-2.0.15/include")
endif()


include_directories(include
                    include/tiled
                    ../external/tinyxml2)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(PetitPoint ${SOURCES})

target_link_libraries(PetitPoint tinyxml2
                                 SDL2
                                 SDL2_image
                                 SDL2_ttf
                                 SDL2_mixer)
