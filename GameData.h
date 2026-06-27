#pragma once

#include "QuestLog.h"
#include "Inventory.h"

class GameData {
public:
    Inventory inventory;
    QuestLog questLog;
    int gold = 500;
};