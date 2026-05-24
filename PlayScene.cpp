#include "PlayScene.h"

PlayScene::PlayScene() {
    camera.x = 0;
    camera.y = 0;
    camera.width = 800;
    camera.height = 600;

    InitMap();
    InitEntities();
}

void PlayScene::InitMap() {
    map = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
}

void PlayScene::InitEntities() {
    Entity player;
    player.x = 100;
    player.y = 100;
    player.width = 50;
    player.height = 50;
    player.type = PLAYER;

    entities.push_back(player);

    Entity npc;
    npc.x = 300;
    npc.y = 200;
    npc.width = 50;
    npc.height = 50;
    npc.type = NPC;

    entities.push_back(npc);
}

void PlayScene::HandleEvents(SDL_Event& event) {
    // 지금은 특별한 이벤트 없음
    // ESC, pause 같은 건 나중에 추가
}

void PlayScene::MovePlayer(Entity& player, int moveX, int moveY){
    MoveAndCollideX(player, moveX);
    MoveAndCollideY(player, moveY);
};

void PlayScene::MoveAndCollideX(Entity& entity, int moveX) {
    if (moveX == 0) return;

    entity.x += moveX;

    SDL_Rect entityRect = {
        entity.x,
        entity.y,
        entity.width,
        entity.height
    };

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 1) continue;

            SDL_Rect wallRect = {
                x * tileSize,
                y * tileSize,
                tileSize,
                tileSize
            };

            if (SDL_HasIntersection(&entityRect, &wallRect)) {
                if (moveX > 0) {
                    entity.x = wallRect.x - entity.width;
                }
                else if (moveX < 0) {
                    entity.x = wallRect.x + wallRect.w;
                }

                entityRect.x = entity.x;
            }
        }
    }
}

void PlayScene::MoveAndCollideY(Entity& entity, int moveY) {
    if (moveY == 0) return;

    entity.y += moveY;

    SDL_Rect entityRect = {
        entity.x,
        entity.y,
        entity.width,
        entity.height
    };

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 1) continue;

            SDL_Rect wallRect = {
                x * tileSize,
                y * tileSize,
                tileSize,
                tileSize
            };

            if (SDL_HasIntersection(&entityRect, &wallRect)) {
                if (moveY > 0) {
                    entity.y = wallRect.y - entity.height;
                }
                else if (moveY < 0) {
                    entity.y = wallRect.y + wallRect.h;
                }

                entityRect.y = entity.y;
            }
        }
    }
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


        MovePlayer(e, moveX, moveY);
        UpdateCamera(e);
    }
}

bool PlayScene::CheckWallCollision(Entity& entity) {
    SDL_Rect entityRect = {
        entity.x,
        entity.y,
        entity.width,
        entity.height
    };

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 1) continue;

            SDL_Rect wallRect = {
                x * tileSize,
                y * tileSize,
                tileSize,
                tileSize
            };

            if (SDL_HasIntersection(&entityRect, &wallRect)) {
                return true;
            }
        }
    }

    return false;
}

void PlayScene::UpdateCamera(Entity& player) {
    camera.x = player.x + player.width / 2 - camera.width / 2;
    camera.y = player.y + player.height / 2 - camera.height / 2;

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;

    int mapWidth = map[0].size() * tileSize;
    int mapHeight = map.size() * tileSize;

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
    RenderMap(renderer);

    for (auto& e : entities) {
        e.Render(renderer, camera);
    }
}

void PlayScene::RenderMap(SDL_Renderer* renderer) {
    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 1) continue;

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

