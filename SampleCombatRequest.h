#pragma once

#include <string>

struct SampleCombatRequest
{
    // 전투에 참가할 캐릭터 데이터 ID
    std::string playerCharacterId;
    std::string enemyCharacterId;

    // 필드에 존재하는 적 개체를 구분하기 위한 ID
    //
    // 같은 "slime" 캐릭터가 여러 마리 있을 때
    // 어떤 슬라임과 전투했는지 식별하는 데 사용한다.
    std::string enemyInstanceId;

    // 전투 종료 후 돌아갈 Scene 이름
    std::string returnSceneName = "Play";

    bool IsValid() const {
        return
            !playerCharacterId.empty() &&
            !enemyCharacterId.empty() &&
            !enemyInstanceId.empty() &&
            !returnSceneName.empty();
    }
};