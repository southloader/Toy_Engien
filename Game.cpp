#include "Game.h"


//Game 생성자
void Game::Init() {
    //작동 트리거 true
    running = true;

    currState = GameState::MainMenu;

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

    //텍스트 출력
    if (TTF_Init() == -1) {
        printf("TTF Init Failed: %s\n", TTF_GetError());
    }

    font = TTF_OpenFont("assets/font.ttf", 24);

    if (font == nullptr) {
        printf("Font Load Failed: %s\n", TTF_GetError());
    }

    //씬 구성
    mainMenuScene = new MainMenuScene(font);
    playScene = new PlayScene();
    pauseScene = new PauseScene(font);

    currentScene = mainMenuScene;
}

void Game::Update() {
    currentScene->Update();

    if (currentScene == mainMenuScene) {
        MenuAction action = mainMenuScene->GetAction();

        if (action == MenuAction::StartGame) {
            currentScene = playScene;
            mainMenuScene->ClearAction();
        }
        else if (action == MenuAction::ExitGame) {
            running = false;
            mainMenuScene->ClearAction();
        }
    }
    if (currentScene == pauseScene) {
        PauseAction action = pauseScene->GetAction();
        if (action == PauseAction::Resume) {
            currentScene = playScene;
            pauseScene->ClearAction();
        }
        else if (action == PauseAction::MainMenu) {
            currentScene = mainMenuScene;
            pauseScene->ClearAction();
        }
        else if (action == PauseAction::Exit) {
            running = false;
            pauseScene->ClearAction();
        }
    }
}

void Game::Render() {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (currentScene == pauseScene) {
        playScene->Render(renderer);
        pauseScene->Render(renderer);
    } 
    else {
        currentScene->Render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    if (font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();
    delete mainMenuScene;
    delete playScene;

    mainMenuScene = nullptr;
    playScene = nullptr;
    currentScene = nullptr;
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

void Game::HandleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {

            if (event.key.keysym.sym == SDLK_ESCAPE) {

                if (currentScene == playScene) {
                    currentScene = pauseScene;
                }

                else if (currentScene == pauseScene) {
                    currentScene = playScene;
                }
            }
        }
        currentScene->HandleEvents(event);
    }
}


