#include "PlayScene.h"

PlayScene::PlayScene(TextureManager* textureManager) {
    this->textureManager = textureManager;
    request = SceneRequest::None;

    camera.x = 0;
    camera.y = 0;
    camera.width = 800;
    camera.height = 600;

    tileMap = new TileMap(textureManager);
    tileMap->Init();
    InitEntities();
}

void PlayScene::InitEntities() {
    Animation idle;
    idle.AddFrame(textureManager->GetTexture("player"));
    idle.SetFrameDelay(1000);
    playerAnimator.AddAnimation("Idle", idle);
    
    Animation walk;
    walk.AddFrame(textureManager->GetTexture("player_walk1"));
    walk.AddFrame(textureManager->GetTexture("player_walk2"));
    walk.AddFrame(textureManager->GetTexture("player_walk3"));
    walk.SetFrameDelay(150);
    playerAnimator.AddAnimation("Walk", walk);

    Animation run;
    run.AddFrame(textureManager->GetTexture("player_run1"));
    run.AddFrame(textureManager->GetTexture("player_run2"));
    run.AddFrame(textureManager->GetTexture("player_run3"));
    run.AddFrame(textureManager->GetTexture("player_run2"));
    run.SetFrameDelay(100);
    playerAnimator.AddAnimation("Run", run);

    playerAnimator.Play("Idle");

    Entity player;
    player.x = 100;
    player.y = 100;
    player.width = 50;
    player.height = 50;
    player.type = PLAYER;
    player.animator = &playerAnimator;

    entities.push_back(player);

    Entity npc;
    npc.x = 300;
    npc.y = 200;
    npc.width = 50;
    npc.height = 50;
    npc.type = NPC;
    npc.texture = textureManager->GetTexture("Npc");

    entities.push_back(npc);

}

void PlayScene::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            request = SceneRequest::GoToPause;
        }
    }
}

void PlayScene::MovePlayer(Entity& player, int moveX, int moveY){
    MoveAndCollideX(player, moveX);
    MoveAndCollideY(player, moveY);
};

void PlayScene::MoveAndCollideX(Entity& entity, int moveX) {
    if (moveX == 0) return;

    entity.x += moveX;
    tileMap->ResolveCollisionX(entity, moveX);
}

void PlayScene::MoveAndCollideY(Entity& entity, int moveY) {
    if (moveY == 0) return;

    entity.y += moveY;
    tileMap->ResolveCollisionY(entity, moveY);
}

void PlayScene::Update() {
    UpdatePlayer();
}

void PlayScene::UpdatePlayer() {
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    
    for (auto& e : entities) {
        if (e.type != PLAYER) continue;

        int speed = playerSpeed;

        if(keyState[SDL_SCANCODE_LSHIFT]) speed = playerSpeed * 2;
        else speed = playerSpeed;

        int moveX = 0;
        int moveY = 0;

        if (keyState[SDL_SCANCODE_W]) moveY -= speed;
        if (keyState[SDL_SCANCODE_S]) moveY += speed;
        if (keyState[SDL_SCANCODE_A]) moveX -= speed;
        if (keyState[SDL_SCANCODE_D]) moveX += speed;

        if (moveX > 0) {
            e.direction = Direction::Right;
        }
        else if (moveX <0) {
            e.direction = Direction::Left;
        }


        MovePlayer(e, moveX, moveY);
        
        bool isMoving = moveX != 0 || moveY != 0;
        bool isRunning = keyState[SDL_SCANCODE_LSHIFT];

        if (!isMoving) {
            playerAnimator.Play("Idle");
        }
        else if (isRunning) {
            playerAnimator.Play("Run");
        }
        else {
            playerAnimator.Play("Walk");
        }

        playerAnimator.Update();

        UpdateCamera(e);
    }
}

void PlayScene::UpdateCamera(Entity& player) {
    camera.x = player.x + player.width / 2 - camera.width / 2;
    camera.y = player.y + player.height / 2 - camera.height / 2;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;

    int mapWidth = tileMap->GetWidth();
    int mapHeight = tileMap->GetHeight();

    if (camera.x > mapWidth - camera.width) {
        camera.x = mapWidth - camera.width;
    }

    if (camera.y > mapHeight - camera.height) {
        camera.y = mapHeight - camera.height;
    }

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
}

void PlayScene::Render(SDL_Renderer* renderer) {
    tileMap->Render(renderer, camera);
    for (auto& e : entities) {
        e.Render(renderer, camera);
    }
}

SceneRequest PlayScene::GetRequest() {
    return request;
}

void PlayScene::ClearRequest() {
    request = SceneRequest::None;
}

PlayScene::~PlayScene() {
    delete tileMap;
    tileMap = nullptr;
}