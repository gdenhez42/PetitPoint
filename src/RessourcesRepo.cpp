#include "RessourcesRepo.h"

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

    return success;
}

void RessourcesRepo::Free()
{
}
