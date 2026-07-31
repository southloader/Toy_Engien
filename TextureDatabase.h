#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

#include "TextureData.h"

class TextureManager;

class TextureDatabase
{
public:
    static bool Init(
        TextureManager* textureManager,
        const std::string& filePath =
            "data/textures.json"
    );

    static bool LoadFromJson(
        TextureManager* textureManager,
        const std::string& filePath
    );

    static bool Register(
        TextureManager* textureManager,
        const TextureData& textureData
    );

    static bool Contains(
        const std::string& id
    );

    static const TextureData* Get(
        const std::string& id
    );

    static std::size_t GetCount();

    static void Clear();

private:
    static std::unordered_map<
        std::string,
        TextureData
    > textures;
};