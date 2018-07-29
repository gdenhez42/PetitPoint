#include "RessourcesRepo.h"
#include <fstream>
#include <Tiled.h>
#include <assert.h>

namespace
{
    // Textures
    const std::string PETITPOINT = "petitpoint";

    // Fonts
    const std::string TITLESCREEN_FONT = "titlescreenFont";
}

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

    // PetitPoint
    if (success)
    {
        success = m_petitPoint.loadFromFile(window, "Images/PetitPoint.png");
    }

    // Title screen
    if (success)
    {
        success = m_titleFont.Load("Fonts/lazy.ttf", 28);
    }
    if (success)
    {
        SDL_Color textColor = { 0xFF, 0xFF, 0xFF };
        success = m_titleText.loadFromRenderedText( window, m_titleFont, "TBR", textColor );
    }

    // Manoir rooms
    if (success)
    {
        success = m_tileSets["Manoir.tsx"].Init(window, "Maps/Manoir.tsx");
    }
    if (success)
    {
        success = m_tileSets["Flags.tsx"].Init(window, "Maps/Flags.tsx");
    }
    if (success)
    {
        success = m_maps["Manoir_SJ.tmx"].Init(window, *this, "Maps/Manoir_SJ.tmx");
    }

    return success;
}

void RessourcesRepo::Free()
{
}

const pp::TileMap& RessourcesRepo::getMap(const std::string& p_name) const
{
    std::map<std::string, pp::TileMap>::const_iterator it = m_maps.find(p_name);
    return it->second;
}

const pp::TileSet& RessourcesRepo::getTileSet(const std::string& p_name) const
{
    std::map<std::string, pp::TileSet>::const_iterator it = m_tileSets.find(p_name);
    assert(it != m_tileSets.end());
    return it->second;
}
