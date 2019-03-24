cmake_minimum_required (VERSION 3.12.3)
project (PetitPoint)



if (${WIN32})
	link_directories("e:/vc_dev_libs/SDL2-2.0.9/lib/x64"
					 "e:/vc_dev_libs/SDL2_ttf-2.0.15/lib/x64"
					 "e:/vc_dev_libs/SDL2_image-2.0.4/lib/x64"
					 ../external/tinyxml2/debug)

	include_directories("e:/vc_dev_libs/SDL2-2.0.9/include"
	                    "e:/vc_dev_libs/SDL2_image-2.0.4/include"
						"e:/vc_dev_libs/SDL2_ttf-2.0.15/include"
						include
						include/tiled
						../external/tinyxml2)

else()
	link_directories(../external/tinyxml2)
	include_directories(include
						include/tiled
						../external/tinyxml2)

endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(PetitPoint ${SOURCES})

if (${WIN32})

	target_link_libraries(PetitPoint SDL2
                                     SDL2_image
									 SDL2_ttf
									 tinyxml2)

endif()

