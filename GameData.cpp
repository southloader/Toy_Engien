#include "GameData.h"
#include "EventManager.h"

void GameData::SetEventManager(EventManager* eventManager) {
    this->eventManager = eventManager;

    inventory.SetEventManager(eventManager);
}

int GameData::GetGold() const {
    return gold;
}

void GameData::AddGold(int amount){
    if (amount <= 0) {
        return;
    }

    gold += amount;

    if (eventManager != nullptr) {
        eventManager->Emit({
            EventType::GoldChanged,"",gold
        });
    }
}

bool GameData::SpendGold(int amount) {
    if (amount <= 0) {
        return false;
    }

    if (gold < amount) {
        return false;
    }

    gold -= amount;

    if (eventManager != nullptr) {
        eventManager->Emit({
            EventType::GoldChanged,"",gold
        });
    }
    return true;
}

void GameData::RestoreGold(int value) {
    gold = value < 0 ? 0 : value;
}