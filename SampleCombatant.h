#pragma once
#include <string>
#include "CombatStats.h"

enum class SampleCombatTeam {
    Player,
    Enemy
};

class SampleCombatant {
public:
    SampleCombatant() = default;

    SampleCombatant(
        const std::string& characterId,
        SampleCombatTeam team,
        const CombatStats& stats
    );

    const std::string& GetCharacterId() const;

    SampleCombatTeam GetTeam() const;

    CombatStats& GetStats();
    const CombatStats& GetStats() const;

private:
    std::string characterId;
    SampleCombatTeam team = SampleCombatTeam::Enemy;
    CombatStats stats;
};