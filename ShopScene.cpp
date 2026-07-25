#include "ShopScene.h"

#include <algorithm>
#include <cstdio>

#include "ItemDatabase.h"
#include "ShopDatabase.h"

ShopScene::ShopScene(
    TTF_Font* font,
    GameData* gameData,
    const std::string& shopId
)
{
    this->font = font;
    this->gameData = gameData;
    this->shopId = shopId;

    BuildShopUI();
}

void ShopScene::BuildShopUI()
{
    uiManager.buttons.clear();
    buttonEntries.clear();

    const ShopData* shop =
        ShopDatabase::Get(shopId);

    if (shop == nullptr)
    {
        std::printf(
            "[ShopScene] "
            "Could not build shop UI. "
            "Shop not found: %s\n",
            shopId.c_str()
        );

        shopName = "Shop Data Missing";

        panelHeight = 240;

        AddCloseButton(
            panelY + 140
        );

        return;
    }

    shopName = shop->name;

    constexpr int buttonStartY = 170;
    constexpr int buttonGap = 15;
    constexpr int buttonHeight = 55;

    for (
        std::size_t index = 0;
        index < shop->entries.size();
        ++index
    )
    {
        AddProductButton(
            shop->entries[index],
            index
        );
    }

    const int entryCount =
        static_cast<int>(
            shop->entries.size()
        );

    const int closeButtonY =
        buttonStartY +
        entryCount *
            (buttonHeight + buttonGap) +
        10;

    AddCloseButton(closeButtonY);

    // 상품 수에 따라 패널 높이를 조정한다.
    panelHeight =
        closeButtonY -
        panelY +
        90;

    // 현재 창 높이가 600이므로
    // 패널이 화면 아래로 지나치게 벗어나지 않게 제한한다.
    panelHeight =
        std::min(
            panelHeight,
            500
        );

    std::printf(
        "[ShopScene] Built UI for shop '%s': "
        "%zu product button(s).\n",
        shopId.c_str(),
        buttonEntries.size()
    );
}

void ShopScene::AddProductButton(
    const ShopEntry& entry,
    std::size_t index
)
{
    if (!ItemDatabase::Contains(
            entry.itemId
        ))
    {
        std::printf(
            "[ShopScene] "
            "Skipped unknown item: %s\n",
            entry.itemId.c_str()
        );

        return;
    }

    const Item item =
        ItemDatabase::Get(
            entry.itemId
        );

    if (item.id == "none")
    {
        return;
    }

    const std::string buttonId =
        "shop_item_" +
        std::to_string(index);

    constexpr int buttonStartY = 170;
    constexpr int buttonGap = 15;
    constexpr int buttonHeight = 55;

    UIButton productButton;

    productButton.id =
        buttonId;

    productButton.x =
        250;

    productButton.y =
        buttonStartY +
        static_cast<int>(index) *
            (buttonHeight + buttonGap);

    productButton.width =
        300;

    productButton.height =
        buttonHeight;

    productButton.text =
        item.name +
        " - " +
        std::to_string(entry.price) +
        "G";

    productButton.normalColor =
        { 70, 70, 180, 255 };

    productButton.hoverColor =
        { 120, 120, 255, 255 };

    productButton.textColor =
        { 255, 255, 255, 255 };

    uiManager.AddButton(
        productButton
    );

    buttonEntries.emplace(
        buttonId,
        entry
    );
}

void ShopScene::AddCloseButton(
    int y
)
{
    UIButton closeButton;

    closeButton.id =
        "close";

    closeButton.x =
        250;

    closeButton.y =
        y;

    closeButton.width =
        300;

    closeButton.height =
        55;

    closeButton.text =
        "닫기";

    closeButton.normalColor =
        { 180, 70, 70, 255 };

    closeButton.hoverColor =
        { 255, 120, 120, 255 };

    closeButton.textColor =
        { 255, 255, 255, 255 };

    uiManager.AddButton(
        closeButton
    );
}

void ShopScene::Update()
{
    uiManager.Update();
}

void ShopScene::Render(
    SDL_Renderer* renderer
)
{
    if (renderer == nullptr)
    {
        return;
    }

    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );

    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        0,
        160
    );

    SDL_Rect overlay =
    {
        0,
        0,
        800,
        600
    };

    SDL_RenderFillRect(
        renderer,
        &overlay
    );

    SDL_SetRenderDrawColor(
        renderer,
        30,
        30,
        30,
        240
    );

    SDL_Rect panel =
    {
        panelX,
        panelY,
        panelWidth,
        panelHeight
    };

    SDL_RenderFillRect(
        renderer,
        &panel
    );

    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );

    SDL_RenderDrawRect(
        renderer,
        &panel
    );

    RenderText(
        renderer,
        font,
        shopName,
        250,
        100
    );

    if (gameData != nullptr)
    {
        const std::string goldText =
            "보유 골드: " +
            std::to_string(
                gameData->GetGold()
            ) +
            "G";

        RenderText(
            renderer,
            font,
            goldText,
            250,
            135
        );
    }

    uiManager.Render(
        renderer,
        font
    );
}

void ShopScene::BuyItem(
    const std::string& itemId,
    int price
)
{
    if (gameData == nullptr)
    {
        std::printf(
            "[ShopScene] "
            "GameData is null.\n"
        );

        return;
    }

    if (!ItemDatabase::Contains(itemId))
    {
        std::printf(
            "[ShopScene] "
            "Cannot buy unknown item: %s\n",
            itemId.c_str()
        );

        return;
    }

    if (price < 0)
    {
        std::printf(
            "[ShopScene] "
            "Invalid price for item '%s': %d\n",
            itemId.c_str(),
            price
        );

        return;
    }

    const Item item =
        ItemDatabase::Get(itemId);

    if (item.id == "none")
    {
        return;
    }

    if (!gameData->SpendGold(price))
    {
        std::printf(
            "[ShopScene] "
            "Not enough gold. "
            "item=%s, price=%d\n",
            itemId.c_str(),
            price
        );

        return;
    }

    gameData->inventory.AddItem(
        item
    );

    std::printf(
        "[ShopScene] Purchased: "
        "%s, price=%d\n",
        itemId.c_str(),
        price
    );
}

void ShopScene::HandleEvents(
    SDL_Event& event
)
{
    if (
        event.type ==
        SDL_MOUSEBUTTONDOWN
    )
    {
        const int mouseX =
            event.button.x;

        const int mouseY =
            event.button.y;

        const std::string clickedId =
            uiManager.GetClickedButtonId(
                mouseX,
                mouseY
            );

        if (clickedId == "close")
        {
            request =
                SceneRequest::CloseOverlay;

            return;
        }

        const auto found =
            buttonEntries.find(
                clickedId
            );

        if (
            found !=
            buttonEntries.end()
        )
        {
            const ShopEntry& entry =
                found->second;

            BuyItem(
                entry.itemId,
                entry.price
            );
        }
    }

    if (
        event.type ==
        SDL_KEYDOWN
    )
    {
        if (
            event.key.keysym.sym ==
            SDLK_ESCAPE
        )
        {
            request =
                SceneRequest::CloseOverlay;
        }
    }
}

SceneRequest ShopScene::GetRequest()
{
    return request;
}

void ShopScene::ClearRequest()
{
    request =
        SceneRequest::None;
}