# Basic Combat Sample

## 1. 목적

Basic Combat Sample은 Toy Engine에 내장된 필수 전투 시스템이 아니다.

이 샘플은 Toy Engine의 Scene, Database, Entity 식별 정보, 입력 처리, UI, 퀘스트 목표 보고 기능을 조합하여 별도의 전투 규칙을 구현할 수 있음을 보여준다.

사용자는 이 샘플을 다음과 같이 사용할 수 있다.

* 그대로 사용
* 데미지 공식 수정
* 새로운 명령 추가
* 다른 전투 방식으로 교체
* 프로젝트에서 완전히 제거

Basic Combat Sample을 제거해도 Engine Core는 정상적으로 존재할 수 있어야 한다.

---

# 2. 주요 구성 요소

## SampleCombatScene

샘플 전투의 화면과 진행 상태를 관리한다.

담당 기능:

* 전투 요청 읽기
* 전투 참가자 생성
* 플레이어 입력 처리
* 플레이어와 적의 턴 실행
* 승리 및 패배 판정
* 전투 화면 렌더링
* 전투 결과 반환

## SampleCombatant

한 번의 전투에 참여하는 캐릭터를 표현한다.

보유 데이터:

* 캐릭터 ID
* 소속 팀
* 전투 능력치

`SampleCombatant`는 월드 객체인 `Entity`와 독립적이다.

```text
Entity
= 월드에 배치되는 객체

SampleCombatant
= 샘플 전투에 참여하는 객체
```

## CombatStats

Basic Combat Sample에서 사용하는 능력치 구조다.

현재 제공 능력치:

* 최대 체력
* 현재 체력
* 공격력
* 방어력
* 속도

이 능력치는 Engine Core의 필수 데이터가 아니다.

HP, 공격력 또는 방어력이 존재하지 않는 게임은 다른 전투 데이터 구조를 사용할 수 있다.

## CombatSystem

Basic Combat Sample의 공격 규칙을 담당한다.

현재 기본 데미지 공식:

```text
damage = max(1, attack - defense)
```

CombatSystem은 다음 시스템을 알지 않는다.

* Entity
* PlayScene
* EventManager
* QuestManager
* 필드 적 제거
* 보상 지급

CombatSystem은 공격 결과를 계산하여 `AttackResult`로 반환할 뿐이다.

## SampleCombatDatabase

캐릭터 ID에 대응하는 샘플 전투 능력치를 보관한다.

예:

```cpp
SampleCombatDatabase::GetStats("slime");
```

능력치는 값으로 반환된다.

따라서 같은 캐릭터 데이터를 사용하는 여러 적도 각자 독립적인 현재 HP를 가진다.

## SampleCombatSession

필드 Scene과 SampleCombatScene 사이에서 요청과 결과를 보관한다.

`Game`이 세션을 소유하므로 Scene이 전환되어도 데이터가 유지된다.

---

# 3. 전투 요청

필드에서 전투를 시작할 때 `SampleCombatRequest`를 생성한다.

```cpp
SampleCombatRequest request;

request.playerCharacterId = "player";
request.enemyCharacterId = "slime";
request.enemyInstanceId = "field_slime_01";
request.returnSceneName = "Play";
```

각 값의 의미:

```text
playerCharacterId
= 플레이어가 사용할 전투 데이터 ID

enemyCharacterId
= 적이 사용할 전투 데이터 ID

enemyInstanceId
= 필드에 존재하는 적 개체의 고유 ID

returnSceneName
= 전투 종료 후 돌아갈 Scene 이름
```

`characterId`와 `instanceId`는 서로 다른 목적을 가진다.

```text
characterId = "slime"
instanceId  = "field_slime_01"
```

여러 슬라임은 같은 `characterId`를 사용할 수 있지만 서로 다른 `instanceId`를 가져야 한다.

---

# 4. 전투 시작

필드 Scene은 전투 규칙을 직접 실행하지 않는다.

전투할 대상만 선택한 뒤 Session에 요청을 등록한다.

```cpp
if (sampleCombatSession->Begin(request))
{
    sceneRequest =
        SceneRequest::GoToSampleCombat;
}
```

SampleCombatScene은 활성 요청을 읽고 전투 참가자를 생성한다.

```cpp
player = SampleCombatant(
    request.playerCharacterId,
    SampleCombatTeam::Player,
    SampleCombatDatabase::GetStats(
        request.playerCharacterId
    )
);

enemy = SampleCombatant(
    request.enemyCharacterId,
    SampleCombatTeam::Enemy,
    SampleCombatDatabase::GetStats(
        request.enemyCharacterId
    )
);
```

---

# 5. 현재 샘플 전투 규칙

현재 샘플은 간단한 턴제 전투를 사용한다.

```text
플레이어 턴
→ Space 키로 공격

적이 생존
→ 적이 자동 반격

적 HP가 0
→ Victory

플레이어 HP가 0
→ Defeat
```

승리 또는 패배 후 Enter 키를 누르면 필드 Scene으로 복귀한다.

---

# 6. 전투 결과

SampleCombatScene은 전투가 종료되면 `SampleCombatResult`를 생성한다.

승리 예:

```cpp
result.outcome =
    SampleCombatOutcome::Victory;

result.enemyInstanceId =
    request.enemyInstanceId;

result.defeatedCharacterId =
    enemy.GetCharacterId();
```

패배 예:

```cpp
result.outcome =
    SampleCombatOutcome::Defeat;
```

결과는 Session에 저장된다.

```cpp
sampleCombatSession->Finish(result);
```

---

# 7. 필드에서 결과 처리

필드 Scene은 복귀 후 결과를 한 번만 소비한다.

승리한 경우:

```text
1. enemyInstanceId에 해당하는 필드 Entity 제거
2. defeatedCharacterId를 퀘스트 목표로 보고
3. 전투 결과 알림 표시
4. Session 결과 제거
```

예:

```cpp
questManager->ReportObjective(
    QuestObjectiveType::DefeatTarget,
    result.defeatedCharacterId,
    1
);
```

패배한 경우에는 적을 제거하거나 퀘스트를 진행하지 않는다.

결과를 처리한 뒤에는 반드시 다음 함수를 호출한다.

```cpp
sampleCombatSession->ClearResult();
```

이를 생략하면 같은 전투 결과가 매 프레임 반복 처리될 수 있다.

---

# 8. 퀘스트 연결

QuestManager는 CombatSystem이나 SampleCombatScene을 알지 않는다.

전투 샘플은 승리 결과를 범용 목표 사건으로 변환한다.

```text
Sample Combat의 승리
→ DefeatTarget / slime / 1
→ QuestManager::ReportObjective()
```

따라서 다른 전투 구현도 같은 API를 사용할 수 있다.

```cpp
questManager->ReportObjective(
    QuestObjectiveType::DefeatTarget,
    "slime",
    1
);
```

퀘스트는 적이 어떤 전투 방식으로 쓰러졌는지 알 필요가 없다.

---

# 9. 새로운 샘플 적 추가

## 시각 데이터 등록

`CharacterDatabase`에 캐릭터 데이터를 등록한다.

```cpp
CharacterData enemy("new_enemy");

enemy.AddAnimation(
    "Idle",
    { "new_enemy_idle" },
    1000
);

enemy.SetDefaultAnimation("Idle");

CharacterDatabase::Register(enemy);
```

## 전투 데이터 등록

`SampleCombatDatabase`에 전투 능력치를 등록한다.

```cpp
SampleCombatDatabase::Register(
    "new_enemy",
    CombatStats(
        50,
        12,
        4,
        6
    )
);
```

## 필드 인스턴스 배치

```cpp
Entity enemy;

enemy.characterId = "new_enemy";
enemy.instanceId = "field_new_enemy_01";
enemy.type = ENEMY;
```

`instanceId`는 필드의 모든 개체 사이에서 고유해야 한다.

---

# 10. 교체 가능한 지점

## 데미지 공식 교체

```cpp
CombatSystem::CalculateDamage()
```

## 플레이어 명령 교체

```cpp
SampleCombatScene::HandleEvents()
SampleCombatScene::ExecutePlayerTurn()
```

## 적 행동 교체

```cpp
SampleCombatScene::ExecuteEnemyTurn()
```

## 승패 조건 교체

```cpp
SampleCombatScene::SetVictory()
SampleCombatScene::SetDefeat()
```

## 전투 UI 교체

```cpp
SampleCombatScene::Render()
SampleCombatScene::RenderCombatant()
SampleCombatScene::RenderBattleText()
```

## 전투 데이터 교체

```text
CombatStats
SampleCombatDatabase
SampleCombatant
```

---

# 11. 샘플 제거

Basic Combat Sample을 사용하지 않는 프로젝트는 다음 기능을 제거할 수 있다.

```text
CombatStats
CombatSystem
SampleCombatant
SampleCombatDatabase
SampleCombatRequest
SampleCombatResult
SampleCombatSession
SampleCombatScene
```

그리고 필드 Scene의 샘플 전투 진입 및 결과 처리 코드도 제거한다.

Engine Core의 Scene, Entity, EventManager, 데이터베이스 구조는 샘플 전투 없이도 존재할 수 있어야 한다.

---

# 12. 현재 제한

현재 SampleCombatScene은 다음 제한을 가진다.

* 플레이어 한 명과 적 한 명만 지원
* 기본 공격만 지원
* 적은 자동으로 반격
* 아이템과 스킬 없음
* 도망 기능 없음
* 전투 중 저장 없음
* `returnSceneName`은 데이터에 존재하지만 현재 복귀 요청은 PlayScene으로 고정됨
* 전투 데이터는 아직 C++ 코드에서 등록됨

이 제한들은 엔진의 한계가 아니라 현재 Basic Combat Sample 구현의 범위다.

---

# 13. 설계 원칙

```text
Engine Core provides tools.

Optional Modules provide reusable features.

Samples demonstrate possible game rules.

The final game rules belong to the creator.
```

Basic Combat Sample은 Toy Engine이 제공하는 정답이 아니다.

Toy Engine으로 구현할 수 있는 여러 가능성 중 하나다.
