# Toy Engine Prototype

게임 엔진의 기본 구조를 이해하기 위해 개발 중인 토이 엔진 프로젝트입니다.

## 목표

* 엔진의 update loop 구조 이해
* UI 요소 식별 구조 설계
* 입력 처리 및 객체 관리 구조 실험
* 유지보수 가능한 코드 구조 학습

## 내가 고민한 부분

* 엔진 사용자와 개발자가 UI 요소를 직관적으로 다룰 수 있도록 id 기반 식별 구조를 구상했습니다.
* 단순 기능 구현보다 내부 구조와 확장성을 이해하는 데 초점을 두었습니다.

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
### 다음 목표

- SceneManager 도입
- ResourceManager 설계
- Texture / Sprite 렌더링 시스템 추가
