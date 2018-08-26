#ifndef RESSOURCESREPO_H
#define RESSOURCESREPO_H

#include "LFont.h"
#include "LTexture.h"
#include <string>
#include <map>

namespace pp {
    class TileSet;
    class TileMap;
}

/*************************************************
Allow to load and access some resources
(textures, font, music...)
*************************************************/
class RessourcesRepo
{
    public:
        RessourcesRepo();
        virtual ~RessourcesRepo();

        bool Load(const LWindow& window);
        void Free();

        const LTexture* getTitleText() const { return &m_titleText; }
        const pp::TileSet& getTileSet(const std::string& p_name) const;
        const pp::TileMap& getMap(const std::string& p_name) const;

    private:
        // Textures
        LTexture m_titleText;

        // Fonts
        LFont m_titleFont;

        // Tilesets
        std::map<std::string,pp::TileSet> m_tileSets;

        // Maps
        std::map<std::string,pp::TileMap> m_maps;

};

#endif // RESSOURCESREPO_H
