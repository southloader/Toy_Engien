# Toy Engine Prototype

게임 엔진의 기본 구조를 이해하기 위해 개발 중인 토이 엔진 프로젝트입니다.
그리고 또한...

# AI-Friendly 2D RPG Engine

A lightweight SDL2-based RPG framework designed to be easy for both developers and AI systems to understand and extend.

## 목표

* 엔진의 update loop 구조 이해
* UI 요소 식별 구조 설계
* 입력 처리 및 객체 관리 구조 실험
* 유지보수 가능한 코드 구조 학습
* "AI가 읽기 쉬운 고수준 API를 구현하여 단순한 AI로도 2D RPG 게임을 만들 수 있도록."

## 개선 중점

* 엔진 사용자와 개발자가 UI 요소를 직관적으로 다룰 수 있도록 id 기반 식별 구조
* 단순 기능 구현보다 내부 구조와 확장성을 이해

## 사용 기술

* 사용 언어: C++
* 개발 환경: vs code, gnu 
* 현재 진행 상태: Game state 설정


## 여지까지 해결한 Critical error

* Redefinition Error -> 문어발식으로 연결된 헤더파일이 문제였기에, Game.h를 root로 두는 트리 형식으로 구조를 바꿔 해결함
* ┗-> 추가로 '#pragma once' 를 남발하는 것이 유일한 해답이 아닌 걸 알아냄..

## 개발 기록

### 2026-05-24

오늘은 Scene 구조를 정리하고 `PauseScene`을 추가했다.

### 추가된 기능

- `Scene` 기반 화면 구조 도입
- `MainMenuScene`, `PlayScene`, `PauseScene` 구조 정리
- 플레이 중 `ESC` 입력 시 일시정지 화면 진입
- Pause 메뉴 UI 추가
  - 계속하기
  - 메인 메뉴
  - 게임 종료
- Pause 상태에서 기존 PlayScene 화면 위에 반투명 오버레이 출력

### 개선된 점

- `Game.cpp`의 역할을 줄이고 Scene별 책임을 분리하기 시작함
- UI 버튼 처리와 Scene 전환 흐름이 더 명확해짐
- 앞으로 OptionScene, SceneManager 확장 가능성이 생김

## 개발 기록

### 2026-06-13

#### TextureManager 도입

- SDL_image 연동
- TextureManager 클래스 구현
- 텍스처 로드 및 관리 기능 추가
- 이미지 기반 렌더링 지원

#### Entity 이미지 렌더링 적용

- Entity가 SDL_Texture를 보유하도록 수정
- SDL_RenderCopy 기반 렌더링 적용
- 플레이어를 player.png로 렌더링하도록 변경

#### Tile Texture 적용

- wall.png 추가
- 벽 타일을 이미지로 렌더링하도록 변경

#### TileMap 시스템 분리

PlayScene 내부에 존재하던 맵 관련 기능을 TileMap 클래스로 분리하였다.

분리된 기능:

- 맵 데이터 관리
- 맵 렌더링
- 충돌 검사
- 충돌 해결
- 맵 크기 계산

구조:

```text
PlayScene
 ├── Player
 ├── Camera
 └── TileMap

TileMap
 ├── Map Data
 ├── Render
 ├── Collision
 └── World Size
```
### 다음 목표

- SceneManager 도입
- ResourceManager 설계
- Texture / Sprite 렌더링 시스템 추가


## 개발 기록
### 2026-06-14

#### Animation System 추가

- Animation 클래스 구현
- 프레임 기반 애니메이션 재생 기능 추가
- 애니메이션별 재생 속도 설정 지원

#### Animator 시스템 추가

- 여러 Animation 관리 기능 구현
- 이름 기반 애니메이션 전환 지원

```cpp
animator.Play("Idle");
animator.Play("Walk");
animator.Play("Run");
```
## 개발 기록
### 2026-06-19

#### NPC Behavior System

NPC 행동 시스템 추가

지원 행동:

- Idle
- Patrol
- FollowPlayer

예시:

```cpp
guard.SetBehavior(NPCBehavior::Patrol);

companion.SetBehavior(NPCBehavior::FollowPlayer);
```

## 개발 기록
### 2026-06-20

NPC 선택형 대화 시스템 추가 및 인벤토리 시스템 기반 추가

## 개발기록
### 2026-06-26

### Inventory & Item System Refactoring

#### Item System

* `Item` 구조체 도입
* `id`, `name`, `description` 기반의 아이템 데이터 관리
* 문자열 기반 인벤토리에서 `Item` 객체 기반 인벤토리로 변경

#### ItemDatabase

* `ItemDatabase` 시스템 추가
* 아이템을 ID로 조회하는 방식으로 변경

```cpp
ItemDatabase::Get("potion");
```

* 상점 및 게임 시스템에서 동일한 아이템 데이터를 공유하도록 구조 개선

#### Inventory Refactoring

* `InventorySlot` 구조체 추가

```cpp
struct InventorySlot
{
    Item item;
    int count;
};
```

* 같은 아이템 구매 시 새로운 슬롯을 생성하지 않고 수량 증가
* 아이템 제거 시 수량 감소 후 0개가 되면 슬롯 삭제
* `GetItems()`를 `GetSlots()`로 변경하여 수량 정보까지 제공

#### Inventory UI

* `InventoryScene` Overlay 추가
* `I` 키로 인벤토리 열기/닫기
* 게임 화면 위에 오버레이 형태로 표시
* 인벤토리 아이템 목록을 실시간으로 렌더링
* 아이템 수량(`Potion x3`) 표시 지원

#### GameData

* `Inventory`를 `GameData`로 이동
* 여러 Scene이 동일한 게임 데이터를 공유하도록 구조 개선

```cpp
GameData
├── Inventory
└── Gold
```

#### Shop Integration

* 상점 구매 시 `ItemDatabase`를 통해 아이템 획득
* 구매한 아이템이 즉시 Inventory에 반영
* Inventory UI에서도 실시간 확인 가능

---

### Architecture Improvement

기존 구조

```
Inventory
└── std::vector<Item>
```

변경 후

```
ItemDatabase
        │
        ▼
      Item
        │
        ▼
 InventorySlot
 ├── Item
 └── Count
        │
        ▼
    Inventory
        │
        ▼
 InventoryScene
```

아이템 정의(Item)와 인벤토리 상태(InventorySlot)를 분리하여 확장성과 유지보수성을 개선하였다.

# Development Log

## Quest System Phase 1 Complete

### Quest Abandon System

Added support for abandoning active quests.

Features:

- Abandon active quests
- Prevent abandoning completed quests
- QuestLog removal support
- Dialogue integration

Example:

```cpp
questManager->AbandonQuest(
    "collect_potion"
);
```

---

### Quest Notification UI

Implemented quest-related notifications.

Supported events:

- Quest Accepted
- Quest Complete
- Quest Abandoned

Example:

```cpp
questNotification.Show(
    "Quest Complete!"
);
```

Quest notifications are now integrated with the Dialogue and Quest systems.

---

### Quest Database Auto Registration

Added automatic registration of quests from QuestDatabase.

Previous workflow:

```cpp
questManager->RegisterQuest(
    QuestDatabase::Get(
        "collect_potion"
    )
);
```

New workflow:

```cpp
QuestDatabase::Init();

questManager->RegisterAllFromDatabase();
```

New quests can now be added by modifying only QuestDatabase.

---

# Character Database Refactoring

### CharacterData API

Improved character registration workflow.

Previous workflow:

```cpp
CharacterData player;

player.id = "player";

player.animations["Walk"] =
{
    ...
};
```

New workflow:

```cpp
CharacterData player(
    "player"
);

player.AddAnimation(
    "Idle",
    {
        "player_idle"
    },
    1000
);

player.AddAnimation(
    "Walk",
    {
        "player_walk_1",
        "player_walk_2",
        "player_walk_3"
    },
    120
);

player.SetDefaultAnimation(
    "Idle"
);

CharacterDatabase::Register(
    player
);
```

---

### CharacterDatabase::Register()

Added a dedicated registration API.

```cpp
CharacterDatabase::Register(
    player
);
```

This separates data definition from database storage responsibilities.

---

### CharacterData Builder Style API

Example:

```cpp
CharacterData merchant(
    "merchant"
);

merchant.AddAnimation(
    "Idle",
    {
        "merchant_idle"
    },
    1000
);

merchant.AddAnimation(
    "Walk",
    {
        "walk1",
        "walk2"
    },
    120
);

merchant.SetDefaultAnimation(
    "Idle"
);

CharacterDatabase::Register(
    merchant
);
```

Goals:

- Meaning-driven API design
- AI-friendly asset workflow
- Flexible animation setup
- Minimal file naming constraints
- Minimal folder structure assumptions

---

# Architecture Update

```text
ItemDatabase

QuestDatabase

CharacterDatabase

↓

Entity

↓

Animator

↓

Renderer

↓

UI
```

Quest and Character systems are now fully database-driven.

---

# Quest System Status

```text
Quest System

██████████ 100%
```

Implemented:

- QuestDatabase
- QuestManager
- QuestLog
- Quest Overlay
- Dynamic Dialogue
- Quest Notifications
- Quest Abandon
- Quest Consume System
- Quest Result
- Reward System
- Inventory Integration

Quest System Phase 1 Complete

---

# Next Goals

Planned Features:

- Save / Load
- EventManager
- Combat System
- Equipment System
- Character Auto Loader
- Asset Manifest