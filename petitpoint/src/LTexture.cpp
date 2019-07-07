#include "LTexture.h"

#ifdef WIN32
#include <SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

namespace pp {

    LTexture::LTexture()
    {
        //Initialize
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }

    LTexture::~LTexture()
    {
        //Deallocate
        free();
    }

    bool LTexture::loadFromFile(const LWindow& window, const std::string& path)
    {
        //Get rid of preexisting texture
        free();

        //We must set the renderer before!
        m_pRenderer = window.getRenderer();

        //The final texture
        SDL_Texture* newTexture = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if( loadedSurface == NULL )
            {
                printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
            }
        else
            {
                //Color key image
                SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0x00, 0xFF, 0xFF ) );
                //Create texture from surface pixels
                newTexture = SDL_CreateTextureFromSurface(m_pRenderer, loadedSurface);
                if( newTexture == NULL )
                    {
                        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
                    }
                else
                    {
                        //Get image dimensions
                        mWidth = loadedSurface->w;
                        mHeight = loadedSurface->h;
                    }
                //Get rid of old loaded surface
                SDL_FreeSurface(loadedSurface);
            }
        //Return success
        mTexture = newTexture;
        return mTexture != NULL;
    }
    bool LTexture::loadFromRenderedText(const LWindow& window, const LFont& p_pFont, std::string textureText, SDL_Color textColor )
    {
        //Get rid of preexisting texture
        free();

        //We must set the renderer before!
        m_pRenderer = window.getRenderer();

        //Render text surface
        SDL_Surface* textSurface = TTF_RenderText_Solid(p_pFont.Font(), textureText.c_str(), textColor );
        if( textSurface == NULL )
            {
                printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
            }
        else
            {
                //Create texture from surface pixels
                mTexture = SDL_CreateTextureFromSurface(m_pRenderer, textSurface );
                if( mTexture == NULL )
                    {
                        printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
                    }
                else
                    {
                        //Get image dimensions
                        mWidth = textSurface->w;
                        mHeight = textSurface->h;
                    }
                //Get rid of old surface
                SDL_FreeSurface( textSurface );
            }
        //Return success
        return mTexture != NULL;
    }

    void LTexture::free()
    {
        //Free texture if it exists
        if( mTexture != NULL )
            {
                SDL_DestroyTexture( mTexture );
                mTexture = NULL;
                mWidth = 0;
                mHeight = 0;
            }
    }

    void LTexture::render(int x, int y, const SDL_Rect* clip) const
    {
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { x, y, mWidth, mHeight };

        //Set clip rendering dimensions
        if(clip != NULL)
            {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h;
            }

        //Render to screen
        SDL_RenderCopy( m_pRenderer, mTexture, clip, &renderQuad );
    }

    int LTexture::getWidth() const
    {
        return mWidth;
    }

    int LTexture::getHeight() const
    {
        return mHeight;
    }

}
