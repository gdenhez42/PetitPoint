/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL and standard IO
#include "FakeBackground.h"
#include "PetitPoint.h"
#include "LTexture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

FakeBackground gFakeBackground;
PetitPoint gPetitPoint;

bool init() {
    //Initialization flag
    bool success = true;
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Set texture filtering to linear
    if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        printf( "Warning: Linear texture filtering not enabled!" );
    }

    if (success)
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
    }


    if (success)
    {
        //Create renderer for window
        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
        if( gRenderer == NULL )
        {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
    }

    if (success)
    {
        //Initialize renderer color
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

        //Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if(success && !( IMG_Init( imgFlags ) & imgFlags ))
        {
            printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            success = false;
        }
    }

    return success;
}

/*****************************
Load textures
******************************/
bool loadMedia()
{
    bool success = true;

    if (success)
    {
        success = gFakeBackground.Load(gWindow, gRenderer);
    }

    if (success)
    {
        success = gPetitPoint.Load(gRenderer);
    }

    return success;
}

void close()
{
    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}

int main( int argc, char* args[] )
{
	// Start up SDL and create window
	if( !init() )
	{
        printf( "Failed to initialize!\n" );
    }
    else
    {
        // Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            // quit event
            bool quit = false;

            // event_handler
            SDL_Event e;
            gFakeBackground.Init(0,0);
            gPetitPoint.Init(SCREEN_WIDTH/2 - gPetitPoint.getWidth()/2, SCREEN_HEIGHT/2 - gPetitPoint.getHeight()/2);

            while (!quit)
            {
                int x = gFakeBackground.getX();
                int y = gFakeBackground.getY();

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //User presses a key
                    else if( e.type == SDL_KEYDOWN )
                    {
                        int ppx = gPetitPoint.getX();
                        int ppy = gPetitPoint.getY();
                        int ppw = gPetitPoint.getWidth();
                        int pph = gPetitPoint.getHeight();

                        //Select surfaces based on key press
                        switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
                                if (gFakeBackground.CanMoveThere(ppx,ppy-2,ppw,pph))
                                {
                                    y-=2;
                                }
                                else if (gFakeBackground.CanMoveThere(ppx,ppy-1,ppw,pph))
                                {
                                    y-=1;
                                }
                                break;
                            case SDLK_DOWN:
                                if (gFakeBackground.CanMoveThere(ppx,ppy+2,ppw,pph))
                                {
                                    y+=2;
                                }
                                else if (gFakeBackground.CanMoveThere(ppx,ppy+1,ppw,pph))
                                {
                                    y+=1;
                                }
                                break;
                            case SDLK_LEFT:
                                if (gFakeBackground.CanMoveThere(ppx-2,ppy,ppw,pph))
                                {
                                    x-=2;
                                }
                                else if (gFakeBackground.CanMoveThere(ppx-1,ppy,ppw,pph))
                                {
                                    x-=1;
                                }
                                break;
                            case SDLK_RIGHT:
                                if (gFakeBackground.CanMoveThere(ppx+2,ppy,ppw,pph))
                                {
                                    x+=2;
                                }
                                else if (gFakeBackground.CanMoveThere(ppx+1,ppy,ppw,pph))
                                {
                                    x+=1;
                                }
                                break;
                            default:
                                break;
                        }
                    }
                }

                //Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
				SDL_RenderClear( gRenderer );

                gFakeBackground.Update(x,y);
                gFakeBackground.Render();
                gPetitPoint.Render();

                //Update screen
				SDL_RenderPresent( gRenderer );
            }
        }
    }
    //Free resources and close SDL
    close();
    return 0;
}
