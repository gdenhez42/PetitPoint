cmake_minimum_required (VERSION 3.12.3)


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

add_library(PetitPointLib STATIC src/tiled/Tiled.cpp
                                 src/Enemy.cpp
						         src/GameMachine.cpp
						         src/GameState.cpp
						         src/LAnimation.cpp
						         src/LevelState.cpp
						         src/LFont.cpp
						         src/LMap.cpp
						         src/LTexture.cpp
						         src/LWindow.cpp
						         src/Manoir.cpp
						         src/Personage.cpp
						         src/petitpoint.cpp
						         src/RessourcesRepo.cpp
						         src/TitleScreenState.cpp
								 src/Utilities.cpp)

add_executable(PetitPoint src/main.cpp)


target_link_libraries(PetitPoint PetitPointLib
  SDL2
  SDL2_image
  SDL2_ttf
  tinyxml2)


