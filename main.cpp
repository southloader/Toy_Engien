#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Game.h"
#include "Entity.h"

Uint32 frameStart;
int frameTime;
const int FPS = 60;
const int frameDelay = 1000 / FPS;

int main() {
    Game game;
    Camera cam;
    cam.x = 0;
    cam.y = 0;

    game.Init();
    while (game.running) {
        frameStart = SDL_GetTicks();
        game.HandleEvents();
        game.Update(&cam);
        game.Render(cam);

        

        
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

    }

    game.Clean();

    return 0;
}