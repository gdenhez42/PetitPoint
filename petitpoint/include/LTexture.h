#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "LWindow.h"
#include "LFont.h"


#include <string>

#ifdef WIN32
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

class LTexture
{
    public:
        LTexture();
        virtual ~LTexture();

        //Loads image at specified path
        bool loadFromFile(const LWindow& window, const std::string& path);

        //Creates image from font string
        bool loadFromRenderedText(const LWindow& window, const LFont& p_pFont, std::string textureText, SDL_Color textColor);

        //Renders texture at given point
        void render(int x, int y, const SDL_Rect* clip = NULL) const;
        //Gets image dimensions
        int getWidth() const;
        int getHeight() const;

    private:
        LTexture(const LTexture&);
        LTexture& operator=(LTexture&);

        // A pointer to the renderer
        SDL_Renderer* m_pRenderer;

        //The actual hardware texture
        SDL_Texture* mTexture;
        //Image dimensions
        int mWidth;
        int mHeight;

        //Deallocates texture
        void free();
};

#endif // LTEXTURE_H
