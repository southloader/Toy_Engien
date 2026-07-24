#pragma once

#include <string>

enum class SampleCombatOutcome
{
    None,
    Victory,
    Defeat,
    Escaped,
    Cancelled
};

struct SampleCombatResult {
    SampleCombatOutcome outcome = SampleCombatOutcome::None;

    // 전투 요청에서 전달받았던 필드 적 개체 ID
    std::string enemyInstanceId;

    // 쓰러뜨린 대상의 캐릭터 데이터 ID
    std::string defeatedCharacterId;

    // 돌아갈 Scene
    std::string returnSceneName = "Play";

    bool HasFinished() const {
        return outcome != SampleCombatOutcome::None;
    }
    bool IsVictory() const {
        return outcome == SampleCombatOutcome::Victory;
    }
    bool IsDefeat() const {
        return outcome == SampleCombatOutcome::Defeat;
    }
    bool ShouldRemoveEnemy() const {
        return outcome == SampleCombatOutcome::Victory && !enemyInstanceId.empty();
    }
};