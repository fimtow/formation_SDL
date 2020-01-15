#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#define HEIGHT 480
#define WIDTH 640

#define GAMEOVER_STATE 0
#define RUNNING_STATE 1

typedef struct game_t {
  // player position
  int player_x;
  int player_y;

  // computer position
  int computer_x;
  int computer_y;

  // ball position
  int ball_x;
  int ball_y;
  int vball_x;
  int vball_y;

  // running or gameover
  int state;

  // controls the speed of the ball (difficulty)
  int game_speed;
  float level;
} game_t;

void init_game(game_t* game, int rect_w, int rect_h) {
    game->player_x = WIDTH - rect_w;
    game->player_y = HEIGHT - rect_h;
    game->computer_x = 0;
    game->computer_y = HEIGHT/2 - rect_h/2;
    game->ball_x = WIDTH/2;
    game->ball_y = HEIGHT/2;
    game->state = RUNNING_STATE;
    game->game_speed = 5;
    game->level = game->game_speed;
    game->vball_x = game->game_speed;
    game->vball_y = game->game_speed;
}

void update_gamestate(game_t* game, int mouse_y, int rect_w, int rect_h, int ball_w, int ball_h){
    // ball position update
    game->ball_x += game->vball_x;
    game->ball_y += game->vball_y;

    // set player pos
    game->player_y = mouse_y;
    // collision detection
    //
    if(game->ball_y+ball_h >= HEIGHT) game->vball_y = -game->game_speed;
    if(game->ball_x+ball_w >= WIDTH) game->state = GAMEOVER_STATE;
    if(game->ball_y <= 0) game->vball_y = game->game_speed;
    if(game->ball_x <= 0) game->vball_x = game->game_speed;
    // player and computer collision detection
    //
    if((game->ball_x + ball_w >= WIDTH - rect_w) && ((game->ball_y + ball_h/2)>=game->player_y) && ((game->ball_y + ball_h/2)<=(game->player_y + rect_h)))
       game->vball_x = -game->game_speed;
    if((game->ball_x <= rect_w) && ((game->ball_y + ball_h/2)>=game->computer_y) && ((game->ball_y + ball_h/2)<=(game->computer_y + rect_h)))
       game->vball_x = game->game_speed;
    // computer position so he can always win
    //
    game->computer_y = game->ball_y;

    // increase the game->game_speed of the ball
    game->level += 0.01;
    game->game_speed = (int)game->level;
}

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

    game_t* game = (game_t*) malloc(sizeof(game_t));
    init_game(game, rectplayer.w, rectplayer.h);

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

    // boolean to quit the game
    int stop = 0;
    int mouse_y;

    // game loop
    while(!stop && game->state == RUNNING_STATE)
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

        // mouse position
        SDL_GetMouseState(NULL,&mouse_y);

        update_gamestate(game, mouse_y, rectplayer.w, rectplayer.h, rectball.w, rectball.h);


        // eventually render_game(rend, game)
		    if(game->state == GAMEOVER_STATE)
		    {
		        // initialize TTF
		        TTF_Init();
		
		        // create font
		        TTF_Font *font = TTF_OpenFont("resources/font.ttf",70);
		
		        // create color (will be used with the font)
		        SDL_Color white = {255,255,255};
		
		        // creating surface,texture,Rect as usual
		        surface = TTF_RenderText_Solid(font,"GAME OVER",white);
		        SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surface);
		        SDL_Rect dest;
		        SDL_QueryTexture(text, NULL, NULL, &dest.w, &dest.h);
		        dest.x = WIDTH/2-dest.w/2;
		        dest.y = HEIGHT/2-dest.h/2;
		
		        // draw the image to the window
		        SDL_RenderClear(rend);
		        SDL_RenderCopy(rend, text, NULL, &dest);
		        SDL_RenderPresent(rend);
		
		        // wait a few seconds
		        SDL_Delay(5000);
		
		        // clean up resources
		        TTF_Quit();
		    } else {
            // draw the image to the window
            SDL_RenderClear(rend);
            SDL_RenderCopy(rend, texplayer, NULL, &rectplayer);
            SDL_RenderCopy(rend, texcomputer, NULL, &rectcomputer);
            SDL_RenderCopy(rend, texball, NULL, &rectball);
            SDL_RenderPresent(rend);
    
            // wait to have 60 fps
            SDL_Delay(1000/60);
        }
    }

    // clean up resources before exiting
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texplayer);
    SDL_DestroyTexture(texcomputer);
    SDL_DestroyTexture(texball);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
