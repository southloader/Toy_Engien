#include "PlayScene.h"
#include "CharacterDatabase.h"
#include "SaveManager.h"
#include <cstdio>

PlayScene::PlayScene(TextureManager* textureManager, TTF_Font* font, GameData* gameData, QuestManager* questManager, SaveManager* saveManager) {
    this->textureManager = textureManager;
    this->font = font;
    this->gameData = gameData;
    this->questManager = questManager;
    this->saveManager = saveManager;

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
    player.x = 100;
    player.y = 100;
    player.width = 50;
    player.height = 50;
    player.type = PLAYER;

    player.LoadCharacter(CharacterDatabase::Get("player"), textureManager);

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
    guard.GetEntity().LoadCharacter(CharacterDatabase::Get("npc"), textureManager);


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
    companion.SetPosition(250, 250);
    companion.SetSize(50, 50);

    companion.GetEntity().LoadCharacter(CharacterDatabase::Get("npc"),textureManager);

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
        if (event.key.keysym.sym == SDLK_i) {
            request = SceneRequest::OpenInventory;
        }
        if (event.key.keysym.sym == SDLK_j) {
            request = SceneRequest::OpenQuestLog;
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
        if (event.key.keysym.sym == SDLK_F5) {
            saveManager->Save("save.txt");
        }
        if (event.key.keysym.sym == SDLK_F9){
            saveManager->Load("save.txt");
        }
    }
}

void PlayScene::ProcessDialogueAction() {
    DialogueAction action = dialogueBox.GetRequest();

    if (action == DialogueAction::None) return;

    std::string questId = dialogueBox.GetRequestedQuestId();

    if (action == DialogueAction::OpenShop) {
        request = SceneRequest::OpenShop;
        dialogueBox.Hide();
    }
    else if (action == DialogueAction::CloseDialogue) {
        dialogueBox.Hide();
    }
    else if (action == DialogueAction::ShowText) {
        // 이미 DialogueBox::SelectChoice()에서 resultText가 표시됨
    }
    else if (action == DialogueAction::AcceptQuest) {
        bool accepted = questManager->AcceptQuest(questId);
        Quest* quest = questManager->GetQuest(questId);

        if (quest != nullptr) {
            if (accepted) {
                questNotification.Show("Quest Accepted!");
                dialogueBox.Show("동료", quest->acceptDialogue);
            }
            else {
                dialogueBox.Show("동료", {"이미 받은 의뢰입니다."});
            }
        }
    }
    else if (action == DialogueAction::AbandonQuest) {
        bool abandoned = questManager->AbandonQuest(questId);

        if (abandoned) {
            questNotification.Show("Quest Abandoned!");
            dialogueBox.Show("동료", {"알겠습니다.", "다음에 하셔도 돼요!"});
        }
        else {
            dialogueBox.Show("동료", {"이 의뢰는 취소할 수 없습니다."});
        }
    }
    else if (action == DialogueAction::CompleteQuest) {
        QuestResult result = questManager->CompleteQuest(questId);
        Quest* quest = questManager->GetQuest(questId);

        if (quest == nullptr) {
            dialogueBox.Show("System", {"퀘스트를 찾을 수 없습니다."});
        }
        else if (result == QuestResult::Success) {
            questNotification.Show("Quest Complete!");
            dialogueBox.Show("동료", quest->completeDialogue);
        }
        else if (result == QuestResult::ConditionNotMet) {
            dialogueBox.Show("동료", quest->progressDialogue);
        }
        else if (result == QuestResult::AlreadyCompleted) {
            dialogueBox.Show("동료", quest->alreadyCompletedDialogue);
        }
    }

    dialogueBox.ClearRequest();
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
            if (npc.GetName() == "동료") {
                Quest* quest = questManager->GetQuest("collect_potion");
                if(quest == nullptr) {
                    dialogueBox.ShowChoices(npc.GetName(),
                        "포션 3개를 가져와 주실 수 있나요?",
                        {
                            {
                                "퀘스트를 수락한다",
                                "좋아요, 포션 3개에요!",
                                DialogueAction::AcceptQuest,
                                "collect_potion"
                            },
                            {
                                "거절한다.",
                                "그래요.",
                                DialogueAction::CloseDialogue,
                                ""
                            },
                        }
                    );
                }
                else if (quest->state == QuestState::Active) {
                    if(questManager->CanComplete("collect_potion")) {
                        dialogueBox.ShowChoices(npc.GetName(),
                            "포션을 다 모아오셨군요!",
                            {
                                {
                                    "퀘스트를 완료한다.",
                                    "좋아요, 확인해볼까요?",
                                    DialogueAction::CompleteQuest,
                                    "collect_potion"
                                },
                                {
                                    "아직 보류한다.",
                                    "뭐 더 기다리죠.",
                                    DialogueAction::CloseDialogue,
                                    ""
                                }
                            }
                        );
                    }
                    else {
                        dialogueBox.ShowChoices(npc.GetName(),"아직 포션이 부족해요.",
                        {
                            {
                                "계속 진행한다.",
                                "기다리고 있을게요.",
                                DialogueAction::ShowText,
                                ""
                            },
                            {
                                "퀘스트를 포기한다.",
                                "알겠습니다.",
                                DialogueAction::AbandonQuest,
                                "collect_potion"
                            },
                            {
                                "떠난다.",
                                "다음에 또 오세요.",
                                DialogueAction::CloseDialogue,
                                ""
                            }

                        });
                    }
                }

                else if (quest->state == QuestState::Completed) {
                    dialogueBox.Show(npc.GetName(), quest->alreadyCompletedDialogue);
                }

                return;
            }
            else {
                dialogueBox.ShowChoices(
                    npc.GetName(),
                    "무엇을 도와드릴까요?",
                    {
                        {
                            "마을에 대해 묻는다",
                            "이 마을은 오래된 항구 마을입니다.",
                            DialogueAction::ShowText,
                            ""
                        },
                        {
                            "상점 열기",
                            "상점을 여는 중 ...",
                            DialogueAction::OpenShop,
                            ""
                        },
                        {
                            "떠난다",
                            "다음에 또 오세요.",
                            DialogueAction::CloseDialogue,
                            ""
                        }
                    }
                );
            }
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
    UpdatePlayer();

    Entity* player = GetPlayer();

    for (auto& npc : npcs) {
        npc.Update(player);
    }

    questManager->UpdateQuestProgress();

    ProcessDialogueAction();
    questNotification.Update();
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
    questNotification.Render(renderer, font);
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