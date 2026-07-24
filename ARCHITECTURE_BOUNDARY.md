# Toy Engine Architecture Boundary

## 1. 문서 목적

이 문서는 Toy Engine에서 다음 세 영역의 경계를 정의한다.

1. Engine Core
2. Optional Module
3. Sample Game

새 기능을 추가할 때 해당 기능이 어느 영역에 속하는지 판단하여, 엔진이 특정 게임 규칙을 사용자에게 강요하지 않도록 하는 것이 목적이다.

---

# 2. 프로젝트의 기본 철학

Toy Engine은 완성된 JRPG 제작 툴이 아니라, AI와 개발자가 2D RPG를 만들기 위해 사용할 수 있는 범용 엔진 기반을 목표로 한다.

엔진은 게임 제작에 필요한 구조와 재료를 제공한다.

그러나 다음과 같은 구체적인 게임 규칙을 엔진 코어가 강제로 결정하지 않는다.

* 전투 방식
* 데미지 공식
* 턴 순서
* 승리와 패배 조건
* 스킬 자원
* 캐릭터 성장 공식
* 퀘스트 시작 시점
* 적 선택 방식
* 장비 슬롯 구성

게임 제작자는 엔진이 제공하는 구조를 조합하여 자신만의 게임 규칙을 만들 수 있어야 한다.

---

# 3. Engine Core

Engine Core는 여러 종류의 게임에서 공통으로 사용할 수 있는 기반 기능이다.

Engine Core는 특정 전투 방식이나 특정 게임 장르의 규칙을 알지 않는다.

## Core에 포함되는 기능

### Scene

* Scene 기본 인터페이스
* SceneManager
* Scene 전환
* Overlay Scene
* Scene 요청 처리

### Entity

* 위치
* 크기
* 렌더링
* 애니메이션 연결
* 충돌에 필요한 기본 정보
* 데이터베이스로부터 객체를 생성하는 기능

### Resource

* TextureManager
* Animation
* Animator
* TextRenderer

### Data

* CharacterDatabase
* ItemDatabase
* QuestDatabase
* GameData

### Event

* EventManager
* 이벤트 구독
* 이벤트 구독 해제
* 시스템 사이의 느슨한 연결

### Persistence

* SaveManager
* GameData 저장 및 복원

### UI

* UIButton
* UIManager
* DialogueBox
* 범용 알림 및 텍스트 출력 기능

---

# 4. Optional Module

Optional Module은 많은 게임에서 유용하지만 모든 게임에 필수적이지 않은 기능이다.

사용자는 필요하지 않은 모듈을 제거하거나 다른 구현으로 교체할 수 있어야 한다.

## Optional Module의 예

* Inventory
* Quest
* Shop
* Equipment
* Skill
* Dialogue
* Party
* Reputation
* Crafting

Optional Module은 Engine Core를 사용할 수 있다.

그러나 Engine Core는 Optional Module을 알아서는 안 된다.

## 올바른 의존 방향

```text
Optional Module
        ↓
   Engine Core
```

## 잘못된 의존 방향

```text
Engine Core
        ↓
Optional Module
```

예를 들어 EventManager는 QuestManager를 몰라야 한다.

QuestManager가 EventManager를 이용하는 것은 허용된다.

---

# 5. Sample Game

Sample Game은 엔진의 기능을 조합하여 실제 게임 규칙을 구현하는 예제다.

Sample Game의 구현은 엔진의 필수 규칙이 아니다.

사용자는 샘플을 다음과 같이 다룰 수 있다.

* 그대로 사용
* 일부 수정
* 다른 구현으로 교체
* 완전히 삭제

## Sample Game에 포함되는 기능

### Basic Combat Sample

* SampleCombatScene
* CombatSystem
* CombatStats
* 데미지 공식
* 공격 명령
* 적 행동
* 전투 승패 판정
* 전투 UI
* 전투 결과 생성
* 필드 Scene 복귀

### Sample World

* PlayScene의 테스트 맵
* 테스트 플레이어
* 테스트 슬라임
* 테스트 NPC
* 테스트 퀘스트
* 테스트 상점

### Sample Data

* 플레이어 기본 능력치
* 슬라임 기본 능력치
* 테스트 아이템
* 테스트 퀘스트
* 샘플 대사

---

# 6. 전투 시스템의 위치

Toy Engine은 전투 시스템을 Engine Core의 필수 기능으로 취급하지 않는다.

현재 구현된 다음 기능은 Basic Combat Sample에 속한다.

* CombatStats
* CombatSystem
* AttackResult
* 공격력에서 방어력을 빼는 데미지 공식
* PlayerDamaged
* PlayerDead
* EnemyKilled
* 스페이스바 기본 공격

이 기능들은 엔진으로 전투를 구현할 수 있음을 보여주는 예제다.

사용자는 다른 전투 방식을 구현할 수 있다.

예:

* 필드 액션 전투
* 턴제 전투
* ATB 전투
* 카드 전투
* 퍼즐 전투
* 대화 전투
* 탄막 전투
* 전투가 존재하지 않는 게임

---

# 7. 전투 샘플의 의존 방향

Sample Combat은 Engine Core와 필요한 Optional Module을 사용할 수 있다.

```text
SampleCombatScene
 ├─ Scene
 ├─ SceneManager
 ├─ Entity
 ├─ CharacterDatabase
 ├─ TextureManager
 ├─ EventManager
 └─ Quest Objective API
```

반대로 Engine Core는 다음 클래스를 직접 참조하지 않는다.

```text
CombatSystem
CombatStats
SampleCombatScene
AttackResult
```

## 허용되는 방향

```text
Sample Combat
      ↓
Optional Module
      ↓
 Engine Core
```

## 금지되는 방향

```text
Engine Core
      ↓
Sample Combat
```

---

# 8. 퀘스트와 전투의 연결 원칙

QuestManager는 특정 전투 구현에 직접 의존하지 않는다.

따라서 QuestManager가 다음 이벤트를 전투의 필수 규칙으로 가정하지 않도록 한다.

```text
EnemyKilled
PlayerDamaged
PlayerDead
```

대신 외부 시스템이 퀘스트 목표와 관련된 사건을 범용 인터페이스로 보고한다.

예상 인터페이스:

```cpp
questManager->ReportObjective(
    objectiveType,
    targetId,
    amount
);
```

예:

```cpp
questManager->ReportObjective(
    QuestObjectiveType::Defeat,
    "slime",
    1
);
```

Sample Combat은 적을 쓰러뜨렸을 때 이 인터페이스를 호출한다.

다른 게임은 전투가 아닌 방식으로도 같은 목표를 보고할 수 있다.

예:

```cpp
questManager->ReportObjective(
    QuestObjectiveType::Talk,
    "village_chief",
    1
);
```

```cpp
questManager->ReportObjective(
    QuestObjectiveType::ReachArea,
    "ancient_forest",
    1
);
```

QuestManager는 사건이 어디에서 발생했는지 알 필요가 없다.

---

# 9. Scene 사이 데이터 전달 원칙

SceneManager는 전투 규칙을 알지 않는다.

SceneManager는 Scene 전환과 전환 데이터 전달만 담당한다.

필드에서 샘플 전투로 이동할 때 필요한 데이터는 별도의 전달 객체에 담는다.

예상 구조:

```cpp
struct SampleCombatRequest
{
    std::string playerCharacterId;
    std::string enemyCharacterId;
    std::string enemyInstanceId;
    std::string returnSceneId;
};
```

전투가 종료되면 결과 객체를 만든다.

```cpp
struct SampleCombatResult
{
    bool playerWon;
    bool playerEscaped;

    std::string defeatedEnemyId;
    std::string defeatedEnemyInstanceId;
};
```

SceneManager는 이 데이터의 의미를 해석하지 않는다.

전달만 담당한다.

---

# 10. 기능 분류 질문

새 기능을 만들기 전에 다음 질문을 확인한다.

## 질문 1

이 기능이 없는 게임도 만들 수 있는가?

* 아니오: Engine Core 후보
* 예: Optional Module 또는 Sample 후보

## 질문 2

이 기능이 특정 게임 규칙을 결정하는가?

* 아니오: Engine Core 또는 Optional Module 후보
* 예: Sample 후보

## 질문 3

사용자가 이 기능을 제거했을 때 Engine Core가 컴파일되지 않는가?

* 예: 의존 방향을 재검토해야 함
* 아니오: 적절하게 분리되었을 가능성이 높음

## 질문 4

완전히 다른 규칙을 구현하려면 기존 코드를 뜯어내야 하는가?

* 예: 코어에 지나치게 구체적인 규칙이 들어갔을 가능성이 높음
* 아니오: 확장 가능한 구조일 가능성이 높음

---

# 11. 현재 파일의 잠정 분류

## Engine Core

```text
Animation
Animator
Camera
Entity
EventManager
Scene
SceneManager
SceneRequest
TextureManager
TextRenderer
UIButton
UIManager
GameData
SaveManager
```

## Optional Module

```text
Inventory
InventoryScene
Item
ItemDatabase
ShopScene
Quest
QuestDatabase
QuestLog
QuestManager
QuestScene
QuestNotification
DialogueBox
DialogueAction
NPC
```

## Sample Game

```text
PlayScene
MainMenuScene
PauseScene
CharacterDatabase의 테스트 데이터
QuestDatabase의 테스트 퀘스트
ItemDatabase의 테스트 아이템
테스트 NPC
테스트 맵
```

## Sample Basic Combat

```text
CombatStats
CombatSystem
AttackResult
테스트 슬라임
기본 공격 공식
EnemyKilled
PlayerDamaged
PlayerDead
```

현재 분류는 물리적인 폴더 이동 전의 논리적 분류다.

실제 폴더 이동은 샘플 전투 Scene이 정상 작동한 후 별도의 리팩터링 단계에서 수행한다.

---

# 12. 이번 샘플 전투의 목표

Basic Combat Sample은 다음 흐름을 증명한다.

```text
PlayScene
    ↓
슬라임과 상호작용
    ↓
SampleCombatScene
    ↓
플레이어 공격
    ↓
슬라임 반격
    ↓
승리 또는 패배 판정
    ↓
전투 결과 생성
    ↓
PlayScene 복귀
    ↓
범용 Quest Objective 보고
```

샘플 전투의 목적은 완성도 높은 전투를 제공하는 것이 아니다.

다음 엔진 기능을 조합할 수 있음을 보여주는 것이 목적이다.

* Scene 전환
* 데이터 전달
* CharacterDatabase 조회
* Entity 생성
* 입력 처리
* 상태 변경
* 이벤트 전달
* UI 출력
* 전투 결과 반환
* 퀘스트 진행
* 필드 상태 복원

---

# 13. 최종 원칙

Toy Engine은 게임의 규칙을 대신 결정하지 않는다.

Engine Core는 게임 제작에 필요한 구조를 제공한다.

Optional Module은 자주 사용되는 기능을 선택적으로 제공한다.

Sample Game은 이 구조들을 조합하는 방법을 보여준다.

```text
Core provides tools.

Modules provide reusable features.

Samples demonstrate possibilities.

The game rules belong to the creator.
```
