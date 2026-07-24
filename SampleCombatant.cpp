#include "SampleCombatant.h"

SampleCombatant::SampleCombatant(
    const std::string& characterId,
    SampleCombatTeam team,
    const CombatStats& stats
)
    : characterId(characterId),
      team(team),
      stats(stats)
{
}

const std::string& SampleCombatant::GetCharacterId() const {
    return characterId;
}

SampleCombatTeam SampleCombatant::GetTeam() const {
    return team;
}

CombatStats& SampleCombatant::GetStats() {
    return stats;
}

const CombatStats& SampleCombatant::GetStats() const {
    return stats;
}