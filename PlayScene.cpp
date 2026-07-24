#include "PlayScene.h"
#include "CharacterDatabase.h"
#include "SaveManager.h"
#include "EventManager.h"
#include <cstdio>
#include <algorithm>

PlayScene::PlayScene(
    TextureManager* textureManager, 
    TTF_Font* font, GameData* gameData, 
    QuestManager* questManager, 
    SaveManager* saveManager, 
    EventManager* eventManager,
    SampleCombatSession* sampleCombatSession) {
    this->textureManager = textureManager;
    this->font = font;
    this->gameData = gameData;
    this->questManager = questManager;
    this->saveManager = saveManager;
    this->eventManager = eventManager;
    this->sampleCombatSession = sampleCombatSession;

    request = SceneRequest::None;

    questAcceptedListenerId = eventManager->Subscribe(EventType::QuestAccepted,[this](const Event& event) {questNotification.Show("Quest Accepted!");});
    questCompletedListenerId = eventManager->Subscribe(EventType::QuestCompleted,[this](const Event& event) {questNotification.Show("Quest Complete!");});
    questAbandonedListenerId = eventManager->Subscribe(EventType::QuestAbandoned,[this](const Event& event) {questNotification.Show("Quest Abandoned");});
    gameSavedListenerId = eventManager->Subscribe(EventType::GameSaved,[this](const Event& event) {questNotification.Show("Game Saved!");});
    gameLoadedListenerId = eventManager->Subscribe(EventType::GameLoaded,[this](const Event& event) {questNotification.Show("Game Loaded!");});

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
    
    player.instanceId = "field_player";

    player.x = 100;
    player.y = 100;
    player.width = 50;
    player.height = 50;
    player.type = PLAYER;
    

    player.LoadCharacter(CharacterDatabase::Get("player"), textureManager);
    entities.push_back(player);

    Entity slime;

    slime.instanceId = "field_slime_01";

    slime.x = 400;
    slime.y = 250;
    slime.width = 50;
    slime.height = 50;
    slime.type = ENEMY;
    

    slime.LoadCharacter(CharacterDatabase::Get("slime"),textureManager);
    entities.push_back(slime);
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
    companion.AddQuestInteraction({
        "collect_potion",
        true,  // 제안 가능
        true,  // 완료 보고 가능
        true   // 포기 가능
    });

    npcs.push_back(companion);

    NPC hunter;
    hunter.SetName("사냥꾼");
    hunter.SetPosition(600,250);
    hunter.SetSize(50, 50);

    hunter.GetEntity().LoadCharacter(CharacterDatabase::Get("npc"),textureManager);
    hunter.SetDialogue({
        "나는 이 근방의 몬스터를 사냥하며 살고 있네.",
        "..음? 자네도 잡아볼텐가?"
    });

    hunter.SetBehavior(NPCBehavior::Idle);
    hunter.AddQuestInteraction({
        "kill_slime",
        true,
        true,
        true
    });

    npcs.push_back(hunter);
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
                if (!CheckEnemyInteraction()){
                    CheckNPCInteraction();
                }
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
            bool loaded = saveManager->Load("save.txt");
            if (loaded) {
                questManager->UpdateQuestProgress();
            }
        }
    }
}

bool PlayScene::RemoveEntityByInstanceId(const std::string& instanceId) {
    if (instanceId.empty()) {
        return false;
    }
    const std::size_t previousSize = entities.size();
    entities.erase(
        std::remove_if(
            entities.begin(),
            entities.end(),
            [&instanceId](const Entity& entity) {
                return entity.instanceId == instanceId;
            }
        ),
        entities.end()
    );

    return entities.size() < previousSize;
}

void PlayScene::ProcessCombatResult() {
    if (sampleCombatSession == nullptr) {
        return;
    }

    if (!sampleCombatSession->HasResult()) {
        return;
    }

    const SampleCombatResult result = sampleCombatSession->GetResult();

    if (result.IsVictory()) {
        if (result.ShouldRemoveEnemy()) {
            const bool removed =
                RemoveEntityByInstanceId(
                    result.enemyInstanceId
                );

            if (removed) {
                std::printf(
                    "[PlayScene] Removed defeated "
                    "field entity: %s\n",
                    result.enemyInstanceId.c_str()
                );
            }
            else {
                std::printf(
                    "[PlayScene] Defeated entity "
                    "not found: %s\n",
                    result.enemyInstanceId.c_str()
                );
            }
        }

        if (questManager != nullptr && !result.defeatedCharacterId.empty()) {
            questManager->ReportObjective(
                QuestObjectiveType::DefeatTarget,
                result.defeatedCharacterId,
                1
            );
        }

        questNotification.Show("Enemy Defeated!");
    }
    else if (result.IsDefeat()) {
        questNotification.Show("You were defeated...");
    }
    else if (result.outcome== SampleCombatOutcome::Escaped) {
        questNotification.Show("Escaped from battle.");
    }

    // 같은 결과가 매 프레임 반복 처리되지 않도록
    // 반드시 마지막에 제거한다.
    sampleCombatSession->ClearResult();
}

void PlayScene::ProcessDialogueAction() {
    DialogueAction action =
        dialogueBox.GetRequest();

    if (action == DialogueAction::None){
        return;
    }

    const std::string questId =
        dialogueBox.GetRequestedQuestId();

    std::string speaker =
        dialogueBox.GetSpeakerName();

    if (speaker.empty()){
        speaker = "System";
    }

    if (action == DialogueAction::OpenShop){
        request = SceneRequest::OpenShop;
        dialogueBox.Hide();
    }
    else if (action == DialogueAction::CloseDialogue){
        dialogueBox.Hide();
    }
    else if (action == DialogueAction::ShowText){
        // SelectChoice에서 선택 결과가 이미 표시된다.
    }
    else if (action == DialogueAction::AcceptQuest){
        const bool accepted = questManager->AcceptQuest(questId);

        Quest* quest = questManager->GetQuest(questId);

        if (quest != nullptr){
            if (accepted){
                dialogueBox.Show(
                    speaker,
                    quest->acceptDialogue
                );
            }
            else{
                dialogueBox.Show(
                    speaker,
                    {
                        "이미 받은 의뢰입니다."
                    }
                );
            }
        }
    }
    else if (action == DialogueAction::AbandonQuest){
        const bool abandoned = questManager->AbandonQuest(questId);

        if (abandoned){
            dialogueBox.Show(
                speaker,
                {
                    "알겠네.",
                    "마음이 바뀌면 다시 찾아오게."
                }
            );
        }
        else{
            dialogueBox.Show(
                speaker,
                {
                    "이 의뢰는 포기할 수 없습니다."
                }
            );
        }
    }
    else if (action == DialogueAction::CompleteQuest){
        const QuestResult result = questManager->CompleteQuest(questId);
        Quest* quest = questManager->GetQuest(questId);

        if (quest == nullptr){
            dialogueBox.Show(
                "System",
                {
                    "퀘스트를 찾을 수 없습니다."
                }
            );
        }
        else if (result == QuestResult::Success){
            dialogueBox.Show(
                speaker,
                quest->completeDialogue
            );
        }
        else if (result == QuestResult::ConditionNotMet){
            dialogueBox.Show(
                speaker,
                quest->progressDialogue
            );
        }
        else if (result == QuestResult::AlreadyCompleted){
        dialogueBox.Show(
                speaker,
                quest->alreadyCompletedDialogue
            );
        }
    }

    dialogueBox.ClearRequest();
}

bool PlayScene::CheckEnemyInteraction() {
    Entity* player = GetPlayer();
    if (player == nullptr) {
        return false;
    }

    Entity* nearestEnemy = nullptr;
    int nearestDistanceSquared = 0;
    constexpr int interactionDistance = 80;
    constexpr int interactionDistanceSquared = interactionDistance * interactionDistance;
    const int playerCenterX = player->x + player->width / 2;
    const int playerCenterY = player->y + player->height / 2;

    for (auto& entity : entities) {
        if (entity.type != ENEMY) {
            continue;
        }

        if (entity.instanceId.empty()) {
            continue;
        }

        const int enemyCenterX = entity.x + entity.width / 2;
        const int enemyCenterY = entity.y + entity.height / 2;
        const int dx = playerCenterX - enemyCenterX;
        const int dy = playerCenterY - enemyCenterY;
        const int distanceSquared = dx * dx + dy * dy;

        if (distanceSquared > interactionDistanceSquared){
            continue;
        }
        if ( nearestEnemy == nullptr || distanceSquared < nearestDistanceSquared) {
            nearestEnemy = &entity;
            nearestDistanceSquared =
                distanceSquared;
        }
    }

    if (nearestEnemy == nullptr){
        return false;
    }

    StartCombatWithEnemy(*nearestEnemy);
    return true;
}

void PlayScene::StartCombatWithEnemy(const Entity& enemy) {
    if (sampleCombatSession == nullptr) {
        std::printf(
            "[PlayScene] "
            "SampleCombatSession is null.\n"
        );

        return;
    }

    if (sampleCombatSession->IsActive()) {
        std::printf(
            "[PlayScene] "
            "A combat session is already active.\n"
        );

        return;
    }

    if (enemy.characterId.empty()) {
        std::printf(
            "[PlayScene] "
            "Enemy characterId is empty.\n"
        );

        return;
    }

    if (enemy.instanceId.empty()) {
        std::printf(
            "[PlayScene] "
            "Enemy instanceId is empty.\n"
        );

        return;
    }

    SampleCombatRequest combatRequest;
    combatRequest.playerCharacterId = "player";
    combatRequest.enemyCharacterId = enemy.characterId;
    combatRequest.enemyInstanceId = enemy.instanceId;
    combatRequest.returnSceneName = "Play";

    if (!sampleCombatSession->Begin(combatRequest)) {
        std::printf(
            "[PlayScene] "
            "Failed to begin sample combat.\n"
        );

        return;
    }

    std::printf(
        "[PlayScene] Combat started: "
        "%s, instance=%s\n",
        enemy.characterId.c_str(),
        enemy.instanceId.c_str()
    );

    request = SceneRequest::GoToSampleCombat;
}

void PlayScene::CheckNPCInteraction() {
    Entity* player = GetPlayer();
    if (player == nullptr){
        return;
    }

    for (auto& npc : npcs){
        if (!IsNear(
            *player,
            npc.GetEntity(),
            40
        )){
            continue;
        }

        // 담당 퀘스트가 있는 NPC
        if (npc.HasQuestInteractions()) {
            ShowQuestInteraction(npc);
            return;
        }

        // 담당 퀘스트가 없는 일반 NPC
        dialogueBox.ShowChoices(
            npc.GetName(),
            "무엇을 도와드릴까요?",
            {
                {
                    "마을에 대해 묻는다.",
                    "이 마을은 오래된 항구 마을입니다.",
                    DialogueAction::ShowText,
                    ""
                },
                {
                    "상점 열기",
                    "상점을 여는 중...",
                    DialogueAction::OpenShop,
                    ""
                },
                {
                    "떠난다.",
                    "다음에 또 오세요.",
                    DialogueAction::CloseDialogue,
                    ""
                }
            }
        );

        return;
    }
}

void PlayScene::ShowQuestInteraction(NPC& npc) {
    const auto& interactions = npc.GetQuestInteractions();

    for (const auto& interaction : interactions) {
        if (!interaction.canComplete) {
            continue;
        }
        Quest* quest = questManager->GetQuest(interaction.questId);
        if (quest == nullptr){
            continue;
        }
        if (quest->state != QuestState::Active){
            continue;
        }
        if (!questManager->CanComplete(interaction.questId)){
            continue;
        }

        dialogueBox.ShowChoices(npc.GetName(), 
            "의뢰의 목표를 달성했군.",
            {
                {
                    "완료 보고를 한다.",
                    "의뢰를 완료했습니다.",
                    DialogueAction::CompleteQuest,
                    interaction.questId
                },
                {
                    "나중에 보고한다.",
                    "조금 뒤에 다시 오겠습니다.",
                    DialogueAction::CloseDialogue,
                    ""
                }
            }
        );

        return;
    }

    for (const auto& interaction : interactions)
    {
        Quest* quest = questManager->GetQuest(interaction.questId);

        if (quest == nullptr){
            continue;
        }

        if (quest->state != QuestState::Active){
            continue;
        }

        std::string progressText = "아직 목표를 달성하지 못했습니다.";

        if (!quest->progressDialogue.empty()){
            progressText = quest->progressDialogue.front();
        }
        std::vector<DialogueChoice> choices;

        choices.push_back(
            {
                "계속 진행한다.",
                "알겠습니다.",
                DialogueAction::ShowText,
                ""
            }
        );

        if (interaction.canAbandon)
        {
            choices.push_back(
                {
                    "퀘스트를 포기한다.",
                    "의뢰를 포기하겠습니다.",
                    DialogueAction::AbandonQuest,
                    interaction.questId
                }
            );
        }

        choices.push_back(
            {
                "떠난다.",
                "다음에 다시 오겠습니다.",
                DialogueAction::CloseDialogue,
                ""
            }
        );

        dialogueBox.ShowChoices(
            npc.GetName(),
            progressText,
            choices
        );

        return;
    }

    for (const auto& interaction : interactions)
    {
        if (!interaction.canOffer) {
            continue;
        }

        Quest* acceptedQuest = questManager->GetQuest(interaction.questId);

        if (acceptedQuest != nullptr){
            continue;
        }

        const Quest* definition = questManager->GetQuestDefinition(interaction.questId);

        if (definition == nullptr){
            continue;
        }

        dialogueBox.ShowChoices(
            npc.GetName(),
            definition->offerDialogue,
            {
                {
                    "퀘스트를 수락한다.",
                    "의뢰를 맡겠습니다.",
                    DialogueAction::AcceptQuest,
                    interaction.questId
                },
                {
                    "거절한다.",
                    "지금은 어렵겠습니다.",
                    DialogueAction::CloseDialogue,
                    ""
                }
            }
        );

        return;
    }

    for (const auto& interaction : interactions)
    {
        Quest* quest = questManager->GetQuest(interaction.questId);

        if (quest == nullptr){
            continue;
        }

        if (quest->state != QuestState::Completed){
            continue;
        }

        dialogueBox.Show(
            npc.GetName(),
            quest->alreadyCompletedDialogue
        );

        return;
    }

    // 처리할 퀘스트가 없으면 일반 대화
    dialogueBox.Show(
        npc.GetName(),
        npc.GetDialogue()
    );
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
    ProcessCombatResult();
    UpdatePlayer();
    UpdateEntities();

    Entity* player = GetPlayer();

    for (auto& npc : npcs) {
        npc.Update(player);
    }
    ProcessDialogueAction();
    questNotification.Update();
}

void PlayScene::UpdateEntities() {
    for (auto& entity : entities) {
        entity.animator.Update();
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
    if (eventManager != nullptr) {
        eventManager->Unsubscribe(EventType::QuestAccepted,questAcceptedListenerId);
        eventManager->Unsubscribe(EventType::QuestCompleted,questCompletedListenerId);
        eventManager->Unsubscribe(EventType::QuestAbandoned,questAbandonedListenerId);
        eventManager->Unsubscribe(EventType::GameSaved,gameSavedListenerId);
        eventManager->Unsubscribe(EventType::GameLoaded,gameLoadedListenerId);
    }

    delete tileMap;
    tileMap = nullptr;
}