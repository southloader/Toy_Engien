#include "QuestManager.h"

#include <cstdio>
#include <algorithm>

#include "GameData.h"
#include "ItemDatabase.h"
#include "QuestDatabase.h"
#include "InventorySlot.h"
#include "EventManager.h"

QuestManager::QuestManager(GameData* gameData, EventManager* eventManager) : gameData(gameData), eventManager(eventManager) {
    if (this->eventManager == nullptr) {
        return;
    }

    itemAddedListenerId = this->eventManager->Subscribe(
        EventType::ItemAdded,
        [this](const Event& event) {
                UpdateCollectQuestProgress(
                    event.subjectId
            );
        }
    );

    itemRemovedListenerId = this->eventManager->Subscribe(
        EventType::ItemRemoved,
        [this](const Event& event) {
            UpdateCollectQuestProgress(
                event.subjectId
            );
        }
    );
}

QuestManager::~QuestManager() {
    if (eventManager == nullptr){
        return;
    }
    if (itemAddedListenerId != 0){
        eventManager->Unsubscribe(EventType::ItemAdded, itemAddedListenerId);
    }
    if (itemRemovedListenerId != 0){
        eventManager->Unsubscribe(EventType::ItemRemoved,itemRemovedListenerId);
    }
}

Quest* QuestManager::GetQuest(const std::string& id) {
    return gameData->questLog.GetQuest(id);
}

const Quest* QuestManager::GetQuestDefinition(const std::string& id) const {
    auto found = questDatabase.find(id);
    if (found == questDatabase.end()) {
        return nullptr;
    }

    return &found->second;
}

void QuestManager::RegisterQuest(const Quest& quest) {
    questDatabase[quest.id] = quest;

    printf("Quest registered: %s\n", quest.id.c_str());
}

void QuestManager::RegisterAllFromDatabase() {
    const auto& quests = QuestDatabase::GetAll();

    for (const auto& pair : quests) {
        RegisterQuest(pair.second);
    }
}

bool QuestManager::AcceptQuest(const std::string& id) {
    if (gameData->questLog.HasQuest(id)) {
        printf("Quest already accepted: %s\n", id.c_str());
        return false;
    }

    if (questDatabase.find(id) == questDatabase.end()) {
        printf("Quest not found: %s\n", id.c_str());
        return false;
    }

    Quest quest = questDatabase[id];
    quest.state = QuestState::Active;

    gameData->questLog.AddQuest(quest);

    if (quest.objectiveType == QuestObjectiveType::CollectItem) {
        UpdateCollectQuestProgress(quest.targetId);
    }

    if (eventManager != nullptr) {
        eventManager->Emit({EventType::QuestAccepted, quest.id, 0});
    }
    return true;
}

bool QuestManager::AbandonQuest(const std::string& id) {
    Quest* quest = gameData->questLog.GetQuest(id);

    if (quest == nullptr) {
        printf("Quest not found: %s\n", id.c_str());
        return false;
    }

    if (quest->state == QuestState::Completed) {
        printf("Completed quest cannot be abandoned: %s\n", id.c_str());
        return false;
    }

    gameData->questLog.RemoveQuest(id);

    if (eventManager != nullptr) {
        eventManager->Emit({EventType::QuestAbandoned, id, 0});
    }
    return true;
}

bool QuestManager::CanComplete(const std::string& id) {
    Quest* quest = gameData->questLog.GetQuest(id);

    if (quest == nullptr){
        return false;
    }

    if (quest->state != QuestState::Active){
        return false;
    }

    // CollectItem 목표도 Inventory 이벤트를 통해
    // currentCount가 실제 수량과 동기화된다.
    //
    // 따라서 모든 목표는 최종적으로
    // currentCount와 targetCount로 완료 여부를 판정한다.
    return quest->currentCount >= quest->targetCount;
}

QuestResult QuestManager::CompleteQuest(const std::string& id) {
    Quest* quest = gameData->questLog.GetQuest(id);

    if (quest == nullptr) {

        std::printf(
            "[QuestManager] Cannot complete quest. "
            "Quest is not active: %s\n",
            id.c_str()
        );

        return QuestResult::QuestNotFound;
    }
    if (quest->state == QuestState::Completed) {
        return QuestResult::AlreadyCompleted;
    }
    if (!CanComplete(id)) {
        return QuestResult::ConditionNotMet;
    }
    if (quest->consumeTargetItem && quest->objectiveType == QuestObjectiveType::CollectItem) {
        for (int i = 0; i < quest->targetCount; i++) {
            gameData->inventory.RemoveItem(quest->targetId);
        }
    }
    
    quest->state = QuestState::Completed;
    quest->currentCount = quest->targetCount;
    
    GiveReward(id);

    if (eventManager != nullptr) {
        eventManager->Emit({EventType::QuestCompleted, quest->id, quest->rewardAmount});
    }

    return QuestResult::Success;
}

bool QuestManager::GiveReward(const std::string& id) {
    Quest* quest = gameData->questLog.GetQuest(id);

    if (quest == nullptr) {
        printf("Quest not found in log: %s\n", id.c_str());
        return false;
    }

    if(quest->state != QuestState::Completed) {
        printf("Quest is not completed yet: %s\n", id.c_str());
        return false;
    }

    for (int i = 0; i < quest->rewardAmount; i++) {
        gameData->inventory.AddItem(ItemDatabase::Get(quest->rewardItemId));
    }

    gameData->AddGold(quest->rewardGold);

    printf("Reward given for quest: %s\n", quest->title.c_str());
    printf("item: %s, Gold: %d\n", quest->rewardItemId.c_str(), quest->rewardGold);
    return true;
}

bool QuestManager::IsActive(const std::string& id) {
    Quest* quest = gameData->questLog.GetQuest(id);
    if(quest == nullptr) return false;

    return quest->state == QuestState::Active;
}

bool QuestManager::IsCompleted(const std::string& id) {
    Quest* quest = gameData->questLog.GetQuest(id);
    if (quest == nullptr) return false;

    return quest->state == QuestState::Completed;
}

void QuestManager::UpdateQuestProgress() {
    auto& quests = gameData->questLog.GetMutableQuests();
    for (auto& quest : quests){
        if (quest.state != QuestState::Active){
            continue;
        }
        if (quest.objectiveType != QuestObjectiveType::CollectItem){
            continue;
        }
        const InventorySlot* slot = gameData->inventory.GetSlot(quest.targetId);
        quest.currentCount = slot != nullptr ? slot->count : 0;
        quest.currentCount = std::min(quest.currentCount, quest.targetCount);
    }
}

void QuestManager::UpdateCollectQuestProgress(const std::string& itemId) {
    auto& quests = gameData->questLog.GetMutableQuests();

    for (auto& quest : quests) {
        if (quest.state != QuestState::Active) {
            continue;
        }

        if (quest.objectiveType != QuestObjectiveType::CollectItem) {
            continue;
        }

        if (quest.targetId != itemId) {
            continue;
        }

        const InventorySlot* slot =
            gameData->inventory.GetSlot(itemId);

        quest.currentCount =
            slot != nullptr ? slot->count : 0;

        if (quest.currentCount > quest.targetCount) {
            quest.currentCount = quest.targetCount;
        }
    }
}

void QuestManager::ReportObjective(
    QuestObjectiveType objectiveType,
    const std::string& targetId,
    int amount
) {
    if (targetId.empty()) {
        return;
    }

    // 아이템 수집 목표는 단순히 +1 하는 것이 아니라
    // 현재 Inventory의 실제 수량과 동기화한다.
    if (objectiveType == QuestObjectiveType::CollectItem) {
        UpdateCollectQuestProgress(
            targetId
        );

        return;
    }

    if (amount <= 0) {
        return;
    }

    auto& quests = gameData->questLog.GetMutableQuests();

    for (auto& quest : quests) {
        if (quest.state != QuestState::Active) {
            continue;
        }

        if (quest.objectiveType != objectiveType) {
            continue;
        }

        if (quest.targetId != targetId) {
            continue;
        }

        quest.currentCount += amount;

        quest.currentCount = std::min(quest.currentCount, quest.targetCount);

        std::printf(
            "[Quest Objective] %s: %d / %d\n",
            quest.title.c_str(),
            quest.currentCount,
            quest.targetCount
        );
    }
}