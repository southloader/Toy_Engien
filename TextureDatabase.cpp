#include "TextureDatabase.h"

#include <cstdio>

#include <nlohmann/json.hpp>

#include "JsonFileLoader.h"
#include "TextureManager.h"

std::unordered_map<std::string, TextureData>
    TextureDatabase::textures;

bool TextureDatabase::Init(
    TextureManager* textureManager,
    const std::string& filePath
)
{
    Clear();

    if (textureManager == nullptr)
    {
        std::printf(
            "[TextureDatabase] "
            "TextureManager is null.\n"
        );

        return false;
    }

    if (!LoadFromJson(
            textureManager,
            filePath
        ))
    {
        std::printf(
            "[TextureDatabase] "
            "Initialization failed: %s\n",
            filePath.c_str()
        );

        return false;
    }

    std::printf(
        "[TextureDatabase] Initialized with "
        "%zu texture(s).\n",
        textures.size()
    );

    return true;
}

bool TextureDatabase::LoadFromJson(
    TextureManager* textureManager,
    const std::string& filePath
)
{
    if (textureManager == nullptr)
    {
        std::printf(
            "[TextureDatabase] "
            "TextureManager is null.\n"
        );

        return false;
    }

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
            "[TextureDatabase] "
            "Root JSON value must be an object.\n"
        );

        return false;
    }

    if (
        !root.contains("textures") ||
        !root["textures"].is_array()
    )
    {
        std::printf(
            "[TextureDatabase] "
            "'textures' must be an array.\n"
        );

        return false;
    }

    const nlohmann::json& textureArray =
        root["textures"];

    std::size_t registeredCount = 0;
    std::size_t skippedCount = 0;

    for (
        std::size_t index = 0;
        index < textureArray.size();
        ++index
    )
    {
        const nlohmann::json& textureJson =
            textureArray[index];

        if (!textureJson.is_object())
        {
            std::printf(
                "[TextureDatabase] "
                "Texture at index %zu "
                "must be an object.\n",
                index
            );

            ++skippedCount;
            continue;
        }

        if (
            !textureJson.contains("id") ||
            !textureJson["id"].is_string()
        )
        {
            std::printf(
                "[TextureDatabase] "
                "Texture at index %zu "
                "has no valid string 'id'.\n",
                index
            );

            ++skippedCount;
            continue;
        }

        if (
            !textureJson.contains("path") ||
            !textureJson["path"].is_string()
        )
        {
            const std::string id =
                textureJson["id"]
                    .get<std::string>();

            std::printf(
                "[TextureDatabase] "
                "Texture '%s' has no valid "
                "string 'path'.\n",
                id.c_str()
            );

            ++skippedCount;
            continue;
        }

        TextureData textureData;

        textureData.id =
            textureJson["id"]
                .get<std::string>();

        textureData.path =
            textureJson["path"]
                .get<std::string>();

        if (!Register(
                textureManager,
                textureData
            ))
        {
            ++skippedCount;
            continue;
        }

        ++registeredCount;
    }

    std::printf(
        "[TextureDatabase] JSON load complete: "
        "%zu registered, %zu skipped.\n",
        registeredCount,
        skippedCount
    );

    return registeredCount > 0;
}

bool TextureDatabase::Register(
    TextureManager* textureManager,
    const TextureData& textureData
)
{
    if (textureManager == nullptr)
    {
        std::printf(
            "[TextureDatabase] "
            "TextureManager is null.\n"
        );

        return false;
    }

    if (textureData.id.empty())
    {
        std::printf(
            "[TextureDatabase] "
            "Cannot register an empty texture ID.\n"
        );

        return false;
    }

    if (textureData.path.empty())
    {
        std::printf(
            "[TextureDatabase] "
            "Texture '%s' has an empty path.\n",
            textureData.id.c_str()
        );

        return false;
    }

    if (Contains(textureData.id))
    {
        std::printf(
            "[TextureDatabase] "
            "Duplicate texture ID: %s\n",
            textureData.id.c_str()
        );

        return false;
    }

    if (!textureManager->LoadTexture(
            textureData.id,
            textureData.path
        ))
    {
        std::printf(
            "[TextureDatabase] "
            "Failed to load texture: "
            "id=%s, path=%s\n",
            textureData.id.c_str(),
            textureData.path.c_str()
        );

        return false;
    }

    textures.emplace(
        textureData.id,
        textureData
    );

    std::printf(
        "[TextureDatabase] Registered: "
        "%s -> %s\n",
        textureData.id.c_str(),
        textureData.path.c_str()
    );

    return true;
}

bool TextureDatabase::Contains(
    const std::string& id
)
{
    return
        textures.find(id) !=
        textures.end();
}

const TextureData* TextureDatabase::Get(
    const std::string& id
)
{
    const auto found =
        textures.find(id);

    if (found == textures.end())
    {
        std::printf(
            "[TextureDatabase] "
            "Texture data not found: %s\n",
            id.c_str()
        );

        return nullptr;
    }

    return &found->second;
}

std::size_t TextureDatabase::GetCount()
{
    return textures.size();
}

void TextureDatabase::Clear()
{
    textures.clear();
}