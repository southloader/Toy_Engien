#include "Game.h"


//Game 생성자
void Game::Init() {
    //작동 트리거 true
    running = true;
    tileSize = 50;

    SDL_Init(SDL_INIT_VIDEO);

    //창 띄우는 선언
    window = SDL_CreateWindow(
        "Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, 
        0
    );

    renderer = SDL_CreateRenderer(window, -1, 0);

    //맵 구현 2중 리스트. 그리는 대로 그려지니 참고할 것.
    map = {
        {0, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0}
    };

    //플레이어와 적을 생성. 엔티티들은 엔티티 벡터에 저장.
    for(int i = 0; i < 5; i++){
        Entity e;
        e.x = 50*i;
        e.y = 50*i;
        e.width = 50;
        e.height = 50;
        //타입 별 구분은 엔티티의 type을 이용하자. 
        if(i == 4) e.type = PLAYER;
        else e.type = ENEMY;

        entities.push_back(e);
    }
}

void Game::Update() {
    // 키보드 입력 불러오기
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    for (auto& e : entities) {
        if (e.type == PLAYER) {

            // 충돌 시 좌표 복구를 위한 이전 좌표 저장
            int prevX = e.x;
            int prevY = e.y;

            // 이동
            if (keystate[SDL_SCANCODE_W]) e.y -= 2;
            if (keystate[SDL_SCANCODE_S]) e.y += 2;
            if (keystate[SDL_SCANCODE_A]) e.x -= 2;
            if (keystate[SDL_SCANCODE_D]) e.x += 2;

            // 카메라도 같이 이동
            camera.x = e.x + e.width / 2 - camera.width / 2;
            camera.y = e.y + e.height / 2 - camera.height / 2;

            // 충돌 검사
            for (auto& other : entities) {
                if (&e == &other) continue;

                if (e.CheckCollision(other)) {
                    // 충돌하면 되돌리기
                    e.x = prevX;
                    e.y = prevY;
                    
                }
            }

            //맵 그리기
            for (int y = 0; y < map.size(); y++) {
                for (int x = 0; x < map[y].size(); x++) {

                    if (map[y][x] == 1) {

                        SDL_Rect wall = {
                            x * tileSize,
                            y * tileSize,
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

void Game::Render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {

            if (map[y][x] == 1) {
                SDL_Rect wall = {
                    x * tileSize - camera.x,
                    y * tileSize - camera.y,
                    tileSize,
                    tileSize
                };

                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &wall);
            }
        }
    }
    for (auto& e : entities){
        e.Render(renderer, camera);
    }

    SDL_RenderPresent(renderer);
}

void Game::ShowExitConfirm(){
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "예" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "아니오" }
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "종료 확인",
        "게임을 종료.",
        SDL_arraysize(buttons),
        buttons,
        nullptr
    };

    int buttonid;

    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        return;
    }

    if (buttonid == 1) {
        running = false;
    }

};

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

        if (event.type == SDL_KEYDOWN){   
            if(event.key.keysym.sym == SDLK_ESCAPE){
                ShowExitConfirm();
            }
        }    
    }
}