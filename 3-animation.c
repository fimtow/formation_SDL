#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#define HEIGHT 480
#define WIDTH 640
int main(int argc, char** argv)
{
    // initialize graphics and timer system
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

    // creating the window
    SDL_Window* win = SDL_CreateWindow("Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT, 0);

    // create a renderer, which sets up the graphics hardware
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // change render color to blue nights
    SDL_SetRenderDrawColor(rend,53,59,72,255);

    // load the image into memory using SDL_image library function
    SDL_Surface* surface = IMG_Load("resources/player.png");

    // load the image data into the graphics hardware's memory
    SDL_Texture* texplayer = SDL_CreateTextureFromSurface(rend, surface);

    // player rectangle
    SDL_Rect rectplayer;
    SDL_QueryTexture(texplayer, NULL, NULL, &rectplayer.w, &rectplayer.h);

    // change player size
    rectplayer.w /= 10;
    rectplayer.h /= 10;

    // player initial  position
    rectplayer.x = WIDTH - rectplayer.w;
    rectplayer.y = HEIGHT - rectplayer.h;

    // free the surface from memory
    SDL_FreeSurface(surface);

    while(rectplayer.y >= 0)
    {
        rectplayer.y -= 3;

        // draw the image to the window
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, texplayer, NULL, &rectplayer);
        SDL_RenderPresent(rend);

        // wait to have 60 fps
        SDL_Delay(1000/60);
    }


    // wait a few seconds
    SDL_Delay(5000);

    // clean up resources before exiting
    SDL_DestroyTexture(texplayer);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
