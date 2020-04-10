#include "RessourcesRepo.h"
#include "Utilities.h"
#include <Tiled.h>

namespace
{
}

namespace pp {

  RessourcesRepo::RessourcesRepo()
  {
    //ctor
  }

  RessourcesRepo::~RessourcesRepo()
  {
    //dtor

  }

  bool RessourcesRepo::Load(const LWindow& window)
  {
    bool success = true;

    // Title screen
    if (success) {
      success = m_titleFont.Load("resources/fonts/lazy.ttf", 28);
    }
    if (success) {
      SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
      success = m_titleText.loadFromRenderedText( window, m_titleFont, "TBR", textColor );
    }

    // Images to load
    if (success) {
      success = m_images["Assassin"].loadFromFile(window, "resources/images/Assassin.png");
      success &= m_images["Lore"].loadFromFile(window, "resources/images/Lore.png");
      success &= m_images["Manoir"].loadFromFile(window, "resources/images/Manoir.png");
      success &= m_images["PetitLore"].loadFromFile(window, "resources/images/PetitLore.png");
    }

    // Tilesets to load
    if (success) {
      success = m_tileSets["Assassin"].Init("resources/tilesets/Assassin.tsx");
      success &= m_tileSets["Lore"].Init("resources/tilesets/Lore.tsx");
      success &= m_tileSets["Manoir"].Init("resources/tilesets/Manoir.tsx");
      success &= m_tileSets["PetitLore"].Init("resources/tilesets/PetitLore.tsx");
    }

    // Manoir rooms
    if (success) {
      success = m_maps["manoir_WC"].Init("resources/tilemaps/manoir_WC.tmx");
      success &= m_maps["manoir_bibli"].Init("resources/tilemaps/manoir_bibli.tmx");
      success &= m_maps["manoir_corridor"].Init("resources/tilemaps/manoir_corridor.tmx");
    }

    return success;
  }

  void RessourcesRepo::Free()
  {
  }

  const LTexture& RessourcesRepo::getImage(const std::string& p_texture) const
  {
    std::map<std::string, LTexture>::const_iterator it = m_images.find(p_texture);
    if (it == m_images.end()) {
      Log("Could not find image: " + p_texture);
      exit(-1);
    }
    return it->second;
  }

  const tiled::TileMap& RessourcesRepo::getMap(const std::string& p_name) const
  {
    std::map<std::string, tiled::TileMap>::const_iterator it = m_maps.find(p_name);
    if (it == m_maps.end()) {
      Log("Could not find map: " + p_name);
      exit(-1);
    }
    return it->second;
  }

  const tiled::TileSet& RessourcesRepo::getTileSet(const std::string& p_name) const
  {
    std::map<std::string, tiled::TileSet>::const_iterator it = m_tileSets.find(p_name);
    if (it == m_tileSets.end()) {
      Log("Could not find tileSet: " + p_name);
      exit(-1);
    }
    return it->second;
  }

  std::string RessourcesRepo::getRessourceName(const std::string& p_resourcePath)
  {

    size_t lastSlash = p_resourcePath.find_last_of('/');
    size_t lastDot = p_resourcePath.find_last_of('.');
    size_t len = lastDot - lastSlash - 1;
    return p_resourcePath.substr(lastSlash + 1, len);
  }

}
