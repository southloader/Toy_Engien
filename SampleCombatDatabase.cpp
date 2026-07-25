#include "SampleCombatDatabase.h"

#include <cstdio>

std::unordered_map<std::string, CombatStats> SampleCombatDatabase::combatStatsDatabase;

void SampleCombatDatabase::Init() {
    combatStatsDatabase.clear();

    Register(
        "player",
        CombatStats(
            100, // 최대 체력
            15,  // 공격력
            5,   // 방어력
            10   // 속도
        )
    );

    Register(
        "slime",
        CombatStats(
            30, // 최대 체력
            8,  // 공격력
            2,  // 방어력
            4   // 속도
        )
    );

    Register(
        "golem",
        CombatStats(
            200, // 최대 체력
            50,  // 공격력
            20,  // 방어력
            7   // 속도
        )
    );
}

void SampleCombatDatabase::Register(const std::string& characterId, const CombatStats& stats) {
    if (characterId.empty()) {
        std::printf(
            "[SampleCombatDatabase] "
            "Cannot register an empty ID.\n"
        );

        return;
    }

    combatStatsDatabase[characterId] = stats;
}

bool SampleCombatDatabase::Contains(const std::string& characterId) {
    return
        combatStatsDatabase.find(characterId)
        != combatStatsDatabase.end();
}

CombatStats SampleCombatDatabase::GetStats(const std::string& characterId) {
    auto found = combatStatsDatabase.find(characterId);

    if (found == combatStatsDatabase.end()) {
        std::printf(
            "[SampleCombatDatabase] "
            "Combat stats not found: %s\n",
            characterId.c_str()
        );

        return CombatStats();
    }

    // 값으로 반환하여 각 전투 참가자가
    // 독립적인 현재 HP를 갖게 한다.
    return found->second;
}