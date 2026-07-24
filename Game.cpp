#include "Game.h"
#include "SampleCombatDatabase.h"

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

    //텍스처 매니저
    textureManager = new TextureManager(renderer);
    textureManager->LoadTexture("wall", "assets/brickWall.png");

    //캐릭터 데이터 베이스 초기화
    CharacterDatabase::Init(textureManager);

    // Basic Combat Sample 전용 데이터
    SampleCombatDatabase::Init();
  
    SampleCombatRequest testRequest;

    testRequest.playerCharacterId = "player";
    testRequest.enemyCharacterId = "slime";
    testRequest.enemyInstanceId = "field_slime_01";
    testRequest.returnSceneName = "Play";

    if (sampleCombatSession.Begin(testRequest)) {
        std::printf(
            "[SampleCombatSession] Request created: "
            "%s vs %s, instance=%s\n",
            sampleCombatSession
                .GetRequest()
                .playerCharacterId
                .c_str(),

            sampleCombatSession
                .GetRequest()
                .enemyCharacterId
                .c_str(),

            sampleCombatSession
                .GetRequest()
                .enemyInstanceId
                .c_str()
    );

    sampleCombatSession.Cancel();
}

    //아이템 데이터 베이스 초기화
    ItemDatabase::Init();

    //퀘스트 데이터 베이스 초기화
    QuestDatabase::Init();

    gameData.SetEventManager(&eventManager);

    questManager = new QuestManager(&gameData, &eventManager);

    questManager->RegisterAllFromDatabase();

    //세이브 매니저
    saveManager = new SaveManager(&gameData, &eventManager);

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
    sceneManager.AddScene("MainMenu", new MainMenuScene(font));
    sceneManager.AddScene("Play", new PlayScene(textureManager, font, &gameData, questManager, saveManager,&eventManager, &sampleCombatSession));
    sceneManager.AddScene("Pause", new PauseScene(font));
    sceneManager.AddScene("SampleCombat", new SampleCombatScene(textureManager, font, &sampleCombatSession));
    sceneManager.AddScene("Shop", new ShopScene(font, &gameData));
    sceneManager.AddScene("Inventory", new InventoryScene(font, &gameData));
    sceneManager.AddScene("QuestLog", new QuestScene(font, &gameData));
    sceneManager.ChangeScene("MainMenu");
}

void Game::Update() {
    sceneManager.Update();
    sceneManager.ProcessRequest(running);
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    sceneManager.Render(renderer);
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
    textureManager->Clean();
    delete textureManager;
    textureManager = nullptr;

    TTF_Quit();
    sceneManager.Clean();

    delete questManager;
    questManager = nullptr;

    delete saveManager;
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

        sceneManager.HandleEvents(event);
    }
}
