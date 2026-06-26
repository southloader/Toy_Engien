#include "PlayScene.h"
#include <cstdio>

PlayScene::PlayScene(TextureManager* textureManager, TTF_Font* font, GameData* gameData) {
    this->textureManager = textureManager;
    this->font = font;
    this->gameData = gameData;

    request = SceneRequest::None;

    camera.x = 0;
    camera.y = 0;
    camera.width = 800;
    camera.height = 600;

    tileMap = new TileMap(textureManager);
    tileMap->Init();

    InitEntities();
    InitNPCs();
}

void PlayScene::InitEntities() {

    Entity player;
    Animation idle;
    idle.AddFrame(textureManager->GetTexture("player"));
    idle.SetFrameDelay(1000);
    player.animator.AddAnimation("Idle", idle);
    
    Animation walk;
    walk.AddFrame(textureManager->GetTexture("player_walk1"));
    walk.AddFrame(textureManager->GetTexture("player_walk2"));
    walk.AddFrame(textureManager->GetTexture("player_walk3"));
    walk.SetFrameDelay(150);
    player.animator.AddAnimation("Walk", walk);

    Animation run;
    run.AddFrame(textureManager->GetTexture("player_run1"));
    run.AddFrame(textureManager->GetTexture("player_run2"));
    run.AddFrame(textureManager->GetTexture("player_run3"));
    run.AddFrame(textureManager->GetTexture("player_run2"));
    run.SetFrameDelay(100);
    player.animator.AddAnimation("Run", run);

    player.animator.Play("Idle");
    player.x = 100;
    player.y = 100;
    player.width = 50;
    player.height = 50;
    player.type = PLAYER;

    entities.push_back(player);
}

Entity* PlayScene::GetPlayer() {
    for (auto& e : entities) {
        if (e.type == PLAYER) {
            return &e;
        }
    }

    return nullptr;
}

void PlayScene::InitNPCs() {
    NPC villager;

    villager.SetName("주민");
    villager.SetPosition(300, 200);
    villager.SetSize(50,50);
    villager.SetTexture(textureManager->GetTexture("npc_idle"));

    villager.SetDialogue({
        "안녕하세요",
        "저는 주민입니다.",
        "주민, 다음주민, 이번주민, 격주민",
        "감사합니다. 땡큐",
        "장난이고, E 키를 누르면 대화할 수 있어요."
    });
    npcs.push_back(villager);

    NPC guard;

    guard.SetName("순찰병");
    guard.SetPosition(300, 100);
    guard.SetSize(50, 50);
    guard.SetTexture(textureManager->GetTexture("npc_idle"));

    Animation guardIdle;
    guardIdle.AddFrame(textureManager->GetTexture("npc_idle"));
    guardIdle.SetFrameDelay(1000);

    Animation guardWalk;
    guardWalk.AddFrame(textureManager->GetTexture("npc_walk_1"));
    guardWalk.AddFrame(textureManager->GetTexture("npc_walk_2"));
    guardWalk.AddFrame(textureManager->GetTexture("npc_walk_3"));
    guardWalk.SetFrameDelay(150);

    guard.GetEntity().animator.AddAnimation("Idle", guardIdle);
    guard.GetEntity().animator.AddAnimation("Walk", guardWalk);
    guard.GetEntity().animator.Play("Idle");

    guard.SetDialogue({
        "나는 순찰 중입니다.",
        "마을은 안전합니다."
    });

    guard.SetBehavior(NPCBehavior::Patrol);
    guard.SetPatrolPoints({
        {300, 100},
        {600, 100}
    });
    guard.SetMoveSpeed(2);
    
    npcs.push_back(guard);


    NPC companion;

    companion.SetName("동료");
    companion.SetPosition(150, 200);
    companion.SetSize(50, 50);
    companion.SetTexture(textureManager->GetTexture("npc_idle"));

    Animation companionIdle;
    companionIdle.AddFrame(textureManager->GetTexture("npc_idle"));
    companionIdle.SetFrameDelay(1000);

    Animation companionWalk;
    companionWalk.AddFrame(textureManager->GetTexture("npc_walk_1"));
    companionWalk.AddFrame(textureManager->GetTexture("npc_walk_2"));
    companionWalk.AddFrame(textureManager->GetTexture("npc_walk_3"));
    companionWalk.SetFrameDelay(150);

    companion.GetEntity().animator.AddAnimation("Idle", companionIdle);
    companion.GetEntity().animator.AddAnimation("Walk", companionWalk);
    companion.GetEntity().animator.Play("Idle");

    companion.SetDialogue({
        "제가 따라갈게요.",
        "너무 빨리 가지 마세요!"
    });

    companion.SetBehavior(NPCBehavior::FollowPlayer);
    companion.SetMoveSpeed(3);
    

    npcs.push_back(companion);
}

void PlayScene::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            request = SceneRequest::GoToPause;
        }
        if (event.key.keysym.sym == SDLK_e){
            if (dialogueBox.IsVisible()) {
                dialogueBox.Next();
            }
            else {
                CheckNPCInteraction();
            }
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_i) {
                request = SceneRequest::OpenInventory;
            }
        }       
        if (event.key.keysym.sym == SDLK_1) {
            dialogueBox.SelectChoice(0);
        }
        if (event.key.keysym.sym == SDLK_2) {
            dialogueBox.SelectChoice(1);
        }
        if (event.key.keysym.sym == SDLK_3) {
            dialogueBox.SelectChoice(2);
        }
    }
    DialogueAction action = dialogueBox.GetRequest();

    if(action == DialogueAction::OpenShop) {
        request = SceneRequest::OpenShop;
        dialogueBox.Hide();
        dialogueBox.ClearRequest();
    }
    if(action == DialogueAction::CloseDialogue) {
        dialogueBox.Hide();
        dialogueBox.ClearRequest();
    }
}

void PlayScene::CheckNPCInteraction() {
    Entity* player = nullptr;

    for (auto& e : entities) {
        if (e.type == PLAYER) {
            player = &e;
            break;
        }
    }

    if (player == nullptr) return;

    for (auto& npc : npcs) {
        if (IsNear(*player, npc.GetEntity(), 80)) {
            dialogueBox.ShowChoices(
                npc.GetName(),
                "무엇을 도와드릴까요?",
                {
                    {
                        "마을에 대해 묻는다",
                        "이 마을은 오래된 항구 마을입니다.",
                        DialogueAction::ShowText
                    },
                    {
                        "상점 열기",
                        "상점을 여는 중 ...",
                        DialogueAction::OpenShop
                    },
                    {
                        "떠난다",
                        "다음에 또 오세요.",
                        DialogueAction::CloseDialogue
                    }
                }
            );
            return;
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
    Entity* player = GetPlayer();
    UpdatePlayer();
    for(auto& npc : npcs){
        npc.Update(player);
    }
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
            e.animator.Play("Idle");
        }
        else if (isRunning) {
            e.animator.Play("Run");
        }
        else {
            e.animator.Play("Walk");
        }

        e.animator.Update();

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
    for (auto& npc : npcs) {
        npc.Render(renderer, camera);
    }

    dialogueBox.Render(renderer, font);
}

SceneRequest PlayScene::GetRequest() {
    return request;
}

void PlayScene::ClearRequest() {
    request = SceneRequest::None;
}

bool PlayScene::IsNear(Entity& a, Entity& b, int distance) {
    int ax = a.x + a.width / 2;
    int ay = a.y + a.height / 2;

    int bx = b.x + b.width / 2;
    int by = b.y + b.height / 2;

    int dx = ax - bx;
    int dy = ay - by;

    return dx * dx + dy * dy <= distance * distance;
}

PlayScene::~PlayScene() {
    delete tileMap;
    tileMap = nullptr;
}