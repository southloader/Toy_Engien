#pragma once

#include "QuestLog.h"
#include "Inventory.h"

class EventManager;

class GameData {
public:
    void SetEventManager(EventManager* eventManager);

    int GetGold() const;

    void AddGold(int amount);
    bool SpendGold(int amount);

    void RestoreGold(int value);

    Inventory inventory;
    QuestLog questLog;
    
private:
    int gold = 500;
    EventManager* eventManager = nullptr;
};