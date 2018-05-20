#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL2/SDL.h>
#include <string>

class LTexture
{
    public:
        LTexture();
        virtual ~LTexture();

        //Loads image at specified path
        bool loadFromFile(SDL_Renderer* p_pRenderer, const std::string& path);

        //Renders texture at given point
        void render(int x, int y, SDL_Rect* clip = NULL);
        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
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
