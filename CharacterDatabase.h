#pragma once

#include <string>
#include <unordered_map>

#include "CharacterData.h"
#include "TextureManager.h"

class CharacterDatabase {
public:
    static void Init(TextureManager* textureManager);
    static CharacterData Get(const std::string& id);
    static void Register(const CharacterData& character);

private:
    static std::unordered_map<std::string, CharacterData> characters;
};
