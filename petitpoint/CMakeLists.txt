cmake_minimum_required (VERSION 3.10.2)


if (${WIN32})
  link_directories(
    "e:/vc_dev_libs/SDL2-2.0.9/lib/x64"
    "e:/vc_dev_libs/SDL2_ttf-2.0.15/lib/x64"
    "e:/vc_dev_libs/SDL2_image-2.0.4/lib/x64"
    ../external/tinyxml2/debug)

  include_directories(
    "e:/vc_dev_libs/SDL2-2.0.9/include"
    "e:/vc_dev_libs/SDL2_image-2.0.4/include"
    "e:/vc_dev_libs/SDL2_ttf-2.0.15/include"
    include
    include/decor
    include/tiled
    include/personages
    include/gamestates
    ../external/tinyxml2)

else()
  link_directories(../external/tinyxml2)
  include_directories(include
    include/decor
    include/tiled
    include/personages
    include/gamestates
    ../external/tinyxml2)

endif()

add_library(PetitPointLib STATIC
  src/tiled/Tiled.cpp
  src/personages/Enemy.cpp
  src/personages/Personage.cpp
  src/personages/PetitPoint.cpp
  src/gamestates/LevelState.cpp
  src/gamestates/GameState.cpp
  src/gamestates/TitleScreenState.cpp
  src/gamestates/InventoryState.cpp
  src/decor/LifeBar.cpp
  src/Commands.cpp
  src/GameMachine.cpp
  src/LAnimation.cpp
  src/LFont.cpp
  src/LMap.cpp
  src/LTexture.cpp
  src/LWindow.cpp
  src/Manoir.cpp
  src/RessourcesRepo.cpp
  src/Utilities.cpp)

add_executable(PetitPoint src/main.cpp)


target_link_libraries(PetitPoint PetitPointLib
  SDL2
  SDL2_image
  SDL2_ttf
  tinyxml2)


