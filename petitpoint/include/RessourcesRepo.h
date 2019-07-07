#ifndef RESSOURCESREPO_H
#define RESSOURCESREPO_H

#include "LFont.h"
#include "LTexture.h"
#include <string>
#include <map>

namespace pp {
    namespace tiled {
        class TileSet;
        class TileMap;
    };
}

namespace pp {

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
        const LTexture& getImage(const std::string& p_texture) const;
        const pp::tiled::TileSet& getTileSet(const std::string& p_name) const;
        const pp::tiled::TileMap& getMap(const std::string& p_name) const;

        static std::string getRessourceName(const std::string& p_ResourcePath);

    private:
        // Fonts
        LFont m_titleFont;
        LTexture m_titleText;

        // Textures
        std::map<std::string, LTexture> m_images;

        // Tilesets
        std::map<std::string,pp::tiled::TileSet> m_tileSets;

        // Maps
        std::map<std::string,pp::tiled::TileMap> m_maps;

};

}
#endif // RESSOURCESREPO_H
