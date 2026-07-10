#include "QuestManager.h"
#include "GameData.h"
#include "ItemDatabase.h"
#include "QuestDatabase.h"
#include "InventorySlot.h"
#include "EventManager.h"
#include <cstdio>

QuestManager::QuestManager(GameData* gameData, EventManager* eventManager) {
    this->gameData = gameData;
    this->eventManager = eventManager;

    if (this->eventManager != nullptr) {
        this->eventManager->Subscribe(
            EventType::ItemAdded,
            [this](const Event& event) {UpdateCollectQuestProgress(event.subjectId);}
        );
        this->eventManager->Subscribe(
            EventType::ItemRemoved,
            [this](const Event& event) {UpdateCollectQuestProgress(event.subjectId);}
        );
    }
}

Quest* QuestManager::GetQuest(const std::string& id) {
    return gameData->questLog.GetQuest(id);
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

    if (quest.type == QuestType::CollectItem) {
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

    if(quest == nullptr)
        return false;

    if(quest->type == QuestType::CollectItem) {
        const InventorySlot* slot = gameData->inventory.GetSlot(quest->targetId);
        if(slot == nullptr)
            return false;

        return slot->count >= quest->targetCount;
    }

    return false;
}

QuestResult QuestManager::CompleteQuest(const std::string& id) {
    Quest* quest = gameData->questLog.GetQuest(id);

    if (quest == nullptr) {
        return QuestResult::QuestNotFound;
    }
    if (quest->state == QuestState::Completed) {
        return QuestResult::AlreadyCompleted;
    }
    if (!CanComplete(id)) {
        return QuestResult::ConditionNotMet;
    }
    if (quest->consumeTargetItem && quest->type == QuestType::CollectItem) {
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

    gameData->gold += quest->rewardGold;

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

    for (auto& quest : quests) {
        if (quest.state != QuestState::Active) continue;

        if (quest.type == QuestType::CollectItem) {
            const InventorySlot* slot =
                gameData->inventory.GetSlot(quest.targetId);

            if (slot != nullptr) {
                quest.currentCount = slot->count;
            }
            else {
                quest.currentCount = 0;
            }

            if (quest.currentCount > quest.targetCount) {
                quest.currentCount = quest.targetCount;
            }
        }
    }
}

void QuestManager::UpdateCollectQuestProgress(const std::string& itemId) {
    auto& quests = gameData->questLog.GetMutableQuests();

    for (auto& quest : quests) {
        if (quest.state != QuestState::Active) {
            continue;
        }

        if (quest.type != QuestType::CollectItem) {
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