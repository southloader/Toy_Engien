#include "ShopDatabase.h"

#include <cstdio>

#include <nlohmann/json.hpp>

#include "ItemDatabase.h"
#include "JsonFileLoader.h"

std::unordered_map<std::string, ShopData>
    ShopDatabase::shops;

bool ShopDatabase::Init(
    const std::string& filePath
) {
    Clear();

    if (!LoadFromJson(filePath))
    {
        std::printf(
            "[ShopDatabase] Initialization failed: %s\n",
            filePath.c_str()
        );

        return false;
    }

    std::printf(
        "[ShopDatabase] Initialized with "
        "%zu shop(s).\n",
        shops.size()
    );

    return true;
}

bool ShopDatabase::LoadFromJson(
    const std::string& filePath
)
{
    nlohmann::json root;

    if (!JsonFileLoader::Load(
            filePath,
            root
        ))
    {
        return false;
    }

    if (!root.is_object())
    {
        std::printf(
            "[ShopDatabase] Root must be an object.\n"
        );

        return false;
    }

    if (
        !root.contains("shops") ||
        !root["shops"].is_array()
    )
    {
        std::printf(
            "[ShopDatabase] "
            "'shops' must be an array.\n"
        );

        return false;
    }

    std::size_t registeredCount = 0;
    std::size_t skippedCount = 0;

    for (
        std::size_t shopIndex = 0;
        shopIndex < root["shops"].size();
        ++shopIndex
    )
    {
        const auto& shopJson =
            root["shops"][shopIndex];

        if (!shopJson.is_object())
        {
            std::printf(
                "[ShopDatabase] "
                "Shop at index %zu must be an object.\n",
                shopIndex
            );

            ++skippedCount;
            continue;
        }

        if (
            !shopJson.contains("id") ||
            !shopJson["id"].is_string()
        )
        {
            std::printf(
                "[ShopDatabase] "
                "Shop at index %zu has no valid ID.\n",
                shopIndex
            );

            ++skippedCount;
            continue;
        }

        const std::string shopId =
            shopJson["id"].get<std::string>();

        if (
            !shopJson.contains("name") ||
            !shopJson["name"].is_string()
        )
        {
            std::printf(
                "[ShopDatabase] "
                "Shop '%s' has no valid name.\n",
                shopId.c_str()
            );

            ++skippedCount;
            continue;
        }

        if (
            !shopJson.contains("items") ||
            !shopJson["items"].is_array()
        )
        {
            std::printf(
                "[ShopDatabase] "
                "Shop '%s' has no valid items array.\n",
                shopId.c_str()
            );

            ++skippedCount;
            continue;
        }

        ShopData shop;

        shop.id = shopId;
        shop.name =
            shopJson["name"].get<std::string>();

        for (
            std::size_t itemIndex = 0;
            itemIndex < shopJson["items"].size();
            ++itemIndex
        )
        {
            const auto& entryJson =
                shopJson["items"][itemIndex];

            if (!entryJson.is_object())
            {
                std::printf(
                    "[ShopDatabase] "
                    "Entry %zu in shop '%s' "
                    "must be an object.\n",
                    itemIndex,
                    shop.id.c_str()
                );

                continue;
            }

            if (
                !entryJson.contains("itemId") ||
                !entryJson["itemId"].is_string()
            )
            {
                std::printf(
                    "[ShopDatabase] "
                    "Entry %zu in shop '%s' "
                    "has no valid itemId.\n",
                    itemIndex,
                    shop.id.c_str()
                );

                continue;
            }

            if (
                !entryJson.contains("price") ||
                !entryJson["price"].is_number_integer()
            )
            {
                std::printf(
                    "[ShopDatabase] "
                    "Entry %zu in shop '%s' "
                    "has no valid price.\n",
                    itemIndex,
                    shop.id.c_str()
                );

                continue;
            }

            ShopEntry entry;

            entry.itemId =
                entryJson["itemId"]
                    .get<std::string>();

            entry.price =
                entryJson["price"]
                    .get<int>();

            if (entry.itemId.empty())
            {
                std::printf(
                    "[ShopDatabase] "
                    "Shop '%s' contains an empty itemId.\n",
                    shop.id.c_str()
                );

                continue;
            }

            if (entry.price < 0)
            {
                std::printf(
                    "[ShopDatabase] "
                    "Item '%s' in shop '%s' "
                    "has invalid price: %d\n",
                    entry.itemId.c_str(),
                    shop.id.c_str(),
                    entry.price
                );

                continue;
            }

            if (!ItemDatabase::Contains(entry.itemId))
            {
                std::printf(
                    "[ShopDatabase] "
                    "Shop '%s' references unknown item: %s\n",
                    shop.id.c_str(),
                    entry.itemId.c_str()
                );

                continue;
            }

            shop.entries.push_back(entry);
        }

        if (shop.entries.empty())
        {
            std::printf(
                "[ShopDatabase] "
                "Shop '%s' has no valid entries.\n",
                shop.id.c_str()
            );

            ++skippedCount;
            continue;
        }

        if (!Register(shop))
        {
            ++skippedCount;
            continue;
        }

        ++registeredCount;
    }

    std::printf(
        "[ShopDatabase] JSON load complete: "
        "%zu registered, %zu skipped.\n",
        registeredCount,
        skippedCount
    );

    return registeredCount > 0;
}

bool ShopDatabase::Register(
    const ShopData& shop
)
{
    if (shop.id.empty())
    {
        std::printf(
            "[ShopDatabase] "
            "Cannot register an empty shop ID.\n"
        );

        return false;
    }

    if (shop.name.empty())
    {
        std::printf(
            "[ShopDatabase] "
            "Shop '%s' has an empty name.\n",
            shop.id.c_str()
        );

        return false;
    }

    if (Contains(shop.id))
    {
        std::printf(
            "[ShopDatabase] Duplicate shop ID: %s\n",
            shop.id.c_str()
        );

        return false;
    }

    shops.emplace(
        shop.id,
        shop
    );

    std::printf(
        "[ShopDatabase] Registered: %s (%zu entries)\n",
        shop.id.c_str(),
        shop.entries.size()
    );

    return true;
}

bool ShopDatabase::Contains(
    const std::string& id
)
{
    return shops.find(id) != shops.end();
}

const ShopData* ShopDatabase::Get(
    const std::string& id
)
{
    const auto found = shops.find(id);

    if (found == shops.end())
    {
        std::printf(
            "[ShopDatabase] Shop not found: %s\n",
            id.c_str()
        );

        return nullptr;
    }

    return &found->second;
}

std::size_t ShopDatabase::GetCount()
{
    return shops.size();
}

void ShopDatabase::Clear()
{
    shops.clear();
}