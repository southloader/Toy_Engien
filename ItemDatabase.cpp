#include "ItemDatabase.h"

#include <cstdio>
#include <nlohmann/json.hpp>

#include "JsonFileLoader.h"

std::unordered_map<std::string, Item> ItemDatabase::items;

bool ItemDatabase::Init(const std::string& filePath) {
    Clear();

    const bool loaded = LoadFromJson(filePath);

    if (!loaded)
    {
        std::printf(
            "[ItemDatabase] "
            "Initialization failed: %s\n",
            filePath.c_str()
        );

        return false;
    }

    std::printf(
        "[ItemDatabase] Initialized with "
        "%zu item(s).\n",
        items.size()
    );

    return true;
}

bool ItemDatabase::LoadFromJson(const std::string& filePath) {
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
            "[ItemDatabase] "
            "Root JSON value must be an object: %s\n",
            filePath.c_str()
        );

        return false;
    }

    if (!root.contains("items"))
    {
        std::printf(
            "[ItemDatabase] "
            "Missing 'items' field: %s\n",
            filePath.c_str()
        );

        return false;
    }

    const nlohmann::json& itemArray = root["items"];

    if (!itemArray.is_array())
    {
        std::printf(
            "[ItemDatabase] "
            "'items' must be an array: %s\n",
            filePath.c_str()
        );

        return false;
    }

    std::size_t registeredCount = 0;
    std::size_t skippedCount = 0;

    for (
        std::size_t index = 0;
        index < itemArray.size();
        ++index
    )
    {
        const nlohmann::json& itemData = itemArray[index];

        if (!itemData.is_object())
        {
            std::printf(
                "[ItemDatabase] "
                "Item at index %zu "
                "must be an object.\n",
                index
            );

            ++skippedCount;
            continue;
        }

        if (
            !itemData.contains("id") ||
            !itemData["id"].is_string()
        )
        {
            std::printf(
                "[ItemDatabase] "
                "Item at index %zu "
                "has no valid string 'id'.\n",
                index
            );

            ++skippedCount;
            continue;
        }

        if (
            !itemData.contains("name") ||
            !itemData["name"].is_string()
        )
        {
            std::printf(
                "[ItemDatabase] "
                "Item '%s' has no valid "
                "string 'name'.\n",
                itemData["id"]
                    .get<std::string>()
                    .c_str()
            );

            ++skippedCount;
            continue;
        }

        if (
            !itemData.contains("description") ||
            !itemData["description"].is_string()
        )
        {
            std::printf(
                "[ItemDatabase] "
                "Item '%s' has no valid "
                "string 'description'.\n",
                itemData["id"]
                    .get<std::string>()
                    .c_str()
            );

            ++skippedCount;
            continue;
        }

        if (
            !itemData.contains("maxStack") ||
            !itemData["maxStack"].is_number_integer()
        )
        {
            std::printf(
                "[ItemDatabase] "
                "Item '%s' has no valid "
                "integer 'maxStack'.\n",
                itemData["id"]
                    .get<std::string>()
                    .c_str()
            );

            ++skippedCount;
            continue;
        }

        Item item;

        item.id =
            itemData["id"]
                .get<std::string>();

        item.name =
            itemData["name"]
                .get<std::string>();

        item.description =
            itemData["description"]
                .get<std::string>();

        item.maxStack =
            itemData["maxStack"]
                .get<int>();

        if (!Register(item))
        {
            ++skippedCount;
            continue;
        }

        ++registeredCount;
    }

    std::printf(
        "[ItemDatabase] JSON load complete: "
        "%zu registered, %zu skipped.\n",
        registeredCount,
        skippedCount
    );

    // JSON 구조가 정상이더라도
    // 유효한 아이템이 하나도 없다면 초기화 실패로 본다.
    return registeredCount > 0;
}

bool ItemDatabase::Register(const Item& item)
{
    if (item.id.empty())
    {
        std::printf(
            "[ItemDatabase] "
            "Cannot register an item "
            "with an empty ID.\n"
        );

        return false;
    }

    if (item.name.empty())
    {
        std::printf(
            "[ItemDatabase] "
            "Item '%s' has an empty name.\n",
            item.id.c_str()
        );

        return false;
    }

    if (item.maxStack <= 0)
    {
        std::printf(
            "[ItemDatabase] "
            "Item '%s' has invalid "
            "maxStack: %d\n",
            item.id.c_str(),
            item.maxStack
        );

        return false;
    }

    if (Contains(item.id))
    {
        std::printf(
            "[ItemDatabase] "
            "Duplicate item ID: %s\n",
            item.id.c_str()
        );

        return false;
    }

    items.emplace(
        item.id,
        item
    );

    std::printf(
        "[ItemDatabase] Registered: "
        "%s (%s)\n",
        item.id.c_str(),
        item.name.c_str()
    );

    return true;
}

bool ItemDatabase::Contains(const std::string& id)
{
    return
        items.find(id)
        != items.end();
}

Item ItemDatabase::Get(const std::string& id)
{
    const auto found =
        items.find(id);

    if (found == items.end())
    {
        std::printf(
            "[ItemDatabase] "
            "Item not found: %s\n",
            id.c_str()
        );

        Item empty;

        empty.id = "none";
        empty.name = "Invalid Item";
        empty.description =
            "The requested item was not found.";

        empty.maxStack = 1;

        return empty;
    }

    return found->second;
}

std::size_t ItemDatabase::GetCount() {
    return items.size();
}

void ItemDatabase::Clear() {
    items.clear();
}