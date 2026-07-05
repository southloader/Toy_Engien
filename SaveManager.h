#pragma once

#include <string>

class GameData;

class SaveManager{
public:
    SaveManager(GameData* gameData);

    bool Save(const std::string& filename);

    bool Load(const std::string& filename);

private:
    GameData* gameData;
};