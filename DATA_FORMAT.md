# Toy Engine Data Format

## 1. 공통 규칙

Toy Engine의 게임 데이터는 JSON 파일로 정의한다.

모든 데이터 객체는 고유한 문자열 ID를 가져야 한다.

```json
{
  "id": "potion"
}
```

ID 규칙:

* 빈 문자열은 허용하지 않는다.
* 같은 데이터베이스 안에서 중복될 수 없다.
* 영문 소문자, 숫자, 밑줄 사용을 권장한다.
* 표시용 이름과 데이터 ID를 구분한다.

권장 예:

```text
village_shop
old_sword
field_slime_01
collect_potion
```

잘못된 데이터 하나가 전체 프로그램을 충돌시키면 안 된다.

가능한 경우 잘못된 항목만 건너뛰고 나머지 유효한 데이터를 등록한다.

---

## 2. items.json

아이템의 기본 정보를 정의한다.

```json
{
  "items": [
    {
      "id": "potion",
      "name": "Potion",
      "description": "Restore HP.",
      "maxStack": 99
    }
  ]
}
```

필드:

* `id`: 아이템 고유 ID
* `name`: 화면에 표시되는 이름
* `description`: 아이템 설명
* `maxStack`: 한 슬롯에 보관할 수 있는 최대 수량

`maxStack`은 1 이상이어야 한다.

---

## 3. shops.json

상점과 판매 상품을 정의한다.

```json
{
  "shops": [
    {
      "id": "village_shop",
      "name": "Village Shop",
      "items": [
        {
          "itemId": "potion",
          "price": 50
        }
      ]
    }
  ]
}
```

상점 필드:

* `id`: 상점 고유 ID
* `name`: 화면에 표시되는 상점 이름
* `items`: 판매 항목 배열

판매 항목 필드:

* `itemId`: `items.json`에 등록된 아이템 ID
* `price`: 판매 가격

존재하지 않는 `itemId`는 등록할 수 없다.

가격은 0 이상이어야 한다.

---

## 4. characters.json

캐릭터의 애니메이션 구성을 정의한다.

```json
{
  "characters": [
    {
      "id": "slime",
      "defaultAnimation": "Idle",
      "animations": [
        {
          "name": "Idle",
          "frames": [
            "slime"
          ],
          "frameDelay": 1000
        }
      ]
    }
  ]
}
```

캐릭터 필드:

* `id`: 캐릭터 데이터 고유 ID
* `defaultAnimation`: 캐릭터 생성 시 처음 재생할 애니메이션 이름
* `animations`: 애니메이션 정의 배열

애니메이션 필드:

* `name`: 애니메이션 이름
* `frames`: TextureManager에 등록된 텍스처 ID 배열
* `frameDelay`: 프레임 사이의 시간(ms)

규칙:

* 캐릭터는 적어도 하나의 애니메이션을 가져야 한다.
* 애니메이션 이름은 같은 캐릭터 안에서 중복될 수 없다.
* 각 애니메이션은 적어도 하나의 프레임을 가져야 한다.
* 모든 프레임 ID는 비어 있지 않아야 한다.
* `frameDelay`는 1 이상이어야 한다.
* `defaultAnimation`은 실제 애니메이션 목록에 존재해야 한다.

`frames`에는 이미지 경로가 아니라 TextureManager의 텍스처 ID를 기록한다.

```json
"frames": [
  "player_walk_1",
  "player_walk_2",
  "player_walk_3"
]
```

현재 실제 이미지 경로와 Texture ID의 연결은 C++의 TextureManager 초기화 코드에서 수행한다.

향후 별도의 texture 또는 asset JSON 포맷으로 분리할 수 있다.

## 5. textures.json

TextureManager에 로딩할 이미지 ID와 파일 경로를 정의한다.

```json
{
  "textures": [
    {
      "id": "slime",
      "path": "assets/slime.png"
    }
  ]
}