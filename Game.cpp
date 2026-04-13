#include "Game.h"


//Game 생성자
void Game::Init() {
    //작동 트리거 true
    running = true;
    tileSize = 50;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 
        0
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    map = {
        {0, 0, 0, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0}
    };
    
    for(int i = 0; i < 5; i++){
        Entity e;
        e.x = 50*i;
        e.y = 50*i;
        e.width = 50;
        e.height = 50;
        if(i == 4) e.type = PLAYER;
        else e.type = ENEMY;

        entities.push_back(e);
    }
}

void Game::Update(Camera * camera) {
    // 키보드 입력 불러오기
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    for (auto& e : entities) {
        if (e.type == PLAYER) {

            //화면 중앙 기준 상수.
            camera->x = e.x - 400;
            camera->y = e.y - 300;

            int prevX = e.x;
            int prevY = e.y;

            // 이동
            if (keystate[SDL_SCANCODE_W]) e.y -= 2;
            if (keystate[SDL_SCANCODE_S]) e.y += 2;
            if (keystate[SDL_SCANCODE_A]) e.x -= 2;
            if (keystate[SDL_SCANCODE_D]) e.x += 2;

            // 충돌 검사
            for (auto& other : entities) {
                if (&e == &other) continue;

                if (e.CheckCollision(other)) {
                    // 충돌하면 되돌리기
                    e.x = prevX;
                    e.y = prevY;
                    
                }
            }
            for (int y = 0; y < map.size(); y++) {
                for (int x = 0; x < map[y].size(); x++) {

                    if (map[y][x] == 1) {

                        SDL_Rect wall = {
                            x * tileSize - camera->x,
                            y * tileSize - camera->y,
                            tileSize,
                            tileSize
                        };

                        SDL_Rect playerRect = {
                            e.x, e.y, e.width, e.height
                        };

                        if (SDL_HasIntersection(&playerRect, &wall)) {
                            e.x = prevX;
                            e.y = prevY;
                        }
                    }
                }
            }
        } 
    }
}

void Game::Render(Camera cam){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {

            if (map[y][x] == 1) {
                SDL_Rect wall = {
                    x * tileSize,
                    y * tileSize,
                    tileSize,
                    tileSize
                };

                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &wall);
            }
        }
    }
    for (auto& e : entities){
        e.Render(renderer, cam);
    }

    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}