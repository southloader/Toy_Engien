#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "CombatStats.h"

struct AnimationData {
    std::vector<std::string> textureIds;
    int frameDelay = 150;
};

struct CharacterData {
    std::string id;
    std::unordered_map<std::string, AnimationData> animations;
    std::string defaultAnimation = "Idle";

    CharacterData() = default;
    CombatStats combatStats;

    CharacterData(const std::string& id);
    void AddAnimation(const std::string& name, const std::vector<std::string>& frames, int frameDelay);
    void SetDefaultAnimation(const std::string& name);
    void SetCombatStats(const CombatStats& stats);
};
