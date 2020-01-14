#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#define HEIGHT 480
#define WIDTH 640
int main(int argc, char** argv)
{
    // initialize graphics and timer system
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER);

    // creating the window
    SDL_Window* win = SDL_CreateWindow("Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT, 0);

    // wait a few seconds
    SDL_Delay(5000);

    // clean up resources before exiting
    SDL_DestroyWindow(win);
    SDL_Quit();
}
