#pragma once

#include <string>
#include "EventManager.h"

class GameData;

class SaveManager{
public:
    SaveManager(GameData* gameData, EventManager* eventManager);

    bool Save(const std::string& filename);

    bool Load(const std::string& filename);

private:
    GameData* gameData = nullptr;
    EventManager* eventManager = nullptr;
};