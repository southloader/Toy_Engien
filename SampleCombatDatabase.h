#pragma once

#include <string>
#include <unordered_map>

#include "CombatStats.h"

class SampleCombatDatabase {
public:
    static void Init();

    static void Register(const std::string& characterId, const CombatStats& stats);
    static bool Contains(const std::string& characterId);
    static CombatStats GetStats(const std::string& characterId);

private:
    static std::unordered_map<std::string,CombatStats> combatStatsDatabase;
};