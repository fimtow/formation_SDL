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

    // load the image into memory using SDL_image library function
    surface = IMG_Load("resources/computer.png");

    // load the image data into the graphics hardware's memory
    SDL_Texture* texcomputer = SDL_CreateTextureFromSurface(rend, surface);

    // computer rectangle
    SDL_Rect rectcomputer;
    SDL_QueryTexture(texcomputer, NULL, NULL, &rectcomputer.w, &rectcomputer.h);

    // change computer size
    rectcomputer.w /= 10;
    rectcomputer.h /= 10;

    // computer initial  position
    rectcomputer.x = 0;
    rectcomputer.y = HEIGHT/2 - rectcomputer.h/2;

    // load the image into memory using SDL_image library function
    surface = IMG_Load("resources/ball.png");

    // load the image data into the graphics hardware's memory
    SDL_Texture* texball = SDL_CreateTextureFromSurface(rend, surface);

    // ball rectangle
    SDL_Rect rectball;
    SDL_QueryTexture(texball, NULL, NULL, &rectball.w, &rectball.h);

    // change ball size
    rectball.w /= 14;
    rectball.h /= 14;

    // ball initial  position
    rectball.x = WIDTH/2;
    rectball.y = HEIGHT/2;

    // ball movement vector
    int speed = 5;
    int vectx = speed;
    int vecty = speed;

    // free the surface from memory
    SDL_FreeSurface(surface);

    // boolean to quit the game
    int stop = 0;

    // game loop
    while(!stop)
    {
        // process events
        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                stop = 1;
            }
        }

        // ball position update
        rectball.x += vectx;
        rectball.y += vecty;

        // collision detection
        if(rectball.y+rectball.h >= HEIGHT) vecty = -speed;
        if(rectball.x+rectball.w >= WIDTH) vectx = -speed;
        if(rectball.y <= 0) vecty = speed;
        if(rectball.x <= 0) vectx = speed;

        // player and computer collision detection
        if((rectball.x + rectball.w >= WIDTH - rectplayer.w) && ((rectball.y + rectball.h/2)>=rectplayer.y) && ((rectball.y + rectball.h/2)<=(rectplayer.y + rectplayer.h)))
           vectx = -speed;
        if((rectball.x <= rectcomputer.w) && ((rectball.y + rectball.h/2)>=rectcomputer.y) && ((rectball.y + rectball.h/2)<=(rectcomputer.y + rectcomputer.h)))
           vectx = speed;

        // computer position so he can always win
        rectcomputer.y = rectball.y;

        // mouse position
        SDL_GetMouseState(NULL,&rectplayer.y);

        // draw the image to the window
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend, texplayer, NULL, &rectplayer);
        SDL_RenderCopy(rend, texcomputer, NULL, &rectcomputer);
        SDL_RenderCopy(rend, texball, NULL, &rectball);
        SDL_RenderPresent(rend);

        // wait to have 60 fps
        SDL_Delay(1000/60);
    }

    // clean up resources before exiting
    SDL_DestroyTexture(texplayer);
    SDL_DestroyTexture(texcomputer);
    SDL_DestroyTexture(texball);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
