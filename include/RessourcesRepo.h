#ifndef RESSOURCESREPO_H
#define RESSOURCESREPO_H

#include "LFont.h"
#include "LTexture.h"
#include <string>
#include <map>

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

        const LTexture* getPetitPoint() const { return &m_petitPoint; }
        const LTexture* getTitleText() const { return &m_titleText; }
        const std::string& getMap(const std::string& p_name) const;

    private:
        // Textures
        LTexture m_petitPoint;
        LTexture m_titleText;

        // Fonts
        LFont m_titleFont;

        // Maps
        std::map<std::string,std::string> m_maps;

};

#endif // RESSOURCESREPO_H
