#include "CharacterDatabase.h"
#include "TextureDatabase.h"

#include <cstdio>
#include <unordered_set>
#include <vector>

#include <nlohmann/json.hpp>

#include "JsonFileLoader.h"

std::unordered_map<std::string, CharacterData>
    CharacterDatabase::characters;

bool CharacterDatabase::Init(
    const std::string& filePath
)
{
    Clear();

    if (!LoadFromJson(filePath))
    {
        std::printf(
            "[CharacterDatabase] "
            "Initialization failed: %s\n",
            filePath.c_str()
        );

        return false;
    }

    std::printf(
        "[CharacterDatabase] Initialized "
        "with %zu character(s).\n",
        characters.size()
    );

    return true;
}

bool CharacterDatabase::LoadFromJson(
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
            "[CharacterDatabase] "
            "Root JSON value must be an object.\n"
        );

        return false;
    }

    if (
        !root.contains("characters") ||
        !root["characters"].is_array()
    )
    {
        std::printf(
            "[CharacterDatabase] "
            "'characters' must be an array.\n"
        );

        return false;
    }

    const nlohmann::json& characterArray =
        root["characters"];

    std::size_t registeredCount = 0;
    std::size_t skippedCount = 0;

    for (
        std::size_t characterIndex = 0;
        characterIndex < characterArray.size();
        ++characterIndex
    )
    {
        const nlohmann::json& characterJson =
            characterArray[characterIndex];

        if (!characterJson.is_object())
        {
            std::printf(
                "[CharacterDatabase] "
                "Character at index %zu "
                "must be an object.\n",
                characterIndex
            );

            ++skippedCount;
            continue;
        }

        if (
            !characterJson.contains("id") ||
            !characterJson["id"].is_string()
        )
        {
            std::printf(
                "[CharacterDatabase] "
                "Character at index %zu "
                "has no valid string 'id'.\n",
                characterIndex
            );

            ++skippedCount;
            continue;
        }

        const std::string characterId =
            characterJson["id"]
                .get<std::string>();

        if (characterId.empty())
        {
            std::printf(
                "[CharacterDatabase] "
                "Character at index %zu "
                "has an empty ID.\n",
                characterIndex
            );

            ++skippedCount;
            continue;
        }

        if (
            !characterJson.contains(
                "defaultAnimation"
            ) ||
            !characterJson[
                "defaultAnimation"
            ].is_string()
        )
        {
            std::printf(
                "[CharacterDatabase] "
                "Character '%s' has no valid "
                "defaultAnimation.\n",
                characterId.c_str()
            );

            ++skippedCount;
            continue;
        }

        if (
            !characterJson.contains("animations") ||
            !characterJson["animations"].is_array()
        )
        {
            std::printf(
                "[CharacterDatabase] "
                "Character '%s' has no valid "
                "animations array.\n",
                characterId.c_str()
            );

            ++skippedCount;
            continue;
        }

        const std::string defaultAnimation =
            characterJson["defaultAnimation"]
                .get<std::string>();

        const nlohmann::json& animationArray =
            characterJson["animations"];

        if (animationArray.empty())
        {
            std::printf(
                "[CharacterDatabase] "
                "Character '%s' has no animations.\n",
                characterId.c_str()
            );

            ++skippedCount;
            continue;
        }

        CharacterData character(
            characterId
        );

        std::unordered_set<std::string>
            animationNames;

        bool characterIsValid = true;

        for (
            std::size_t animationIndex = 0;
            animationIndex < animationArray.size();
            ++animationIndex
        )
        {
            const nlohmann::json& animationJson =
                animationArray[animationIndex];

            if (!animationJson.is_object())
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Animation %zu of character '%s' "
                    "must be an object.\n",
                    animationIndex,
                    characterId.c_str()
                );

                characterIsValid = false;
                break;
            }

            if (
                !animationJson.contains("name") ||
                !animationJson["name"].is_string()
            )
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Animation %zu of character '%s' "
                    "has no valid name.\n",
                    animationIndex,
                    characterId.c_str()
                );

                characterIsValid = false;
                break;
            }

            const std::string animationName =
                animationJson["name"]
                    .get<std::string>();

            if (animationName.empty())
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Character '%s' has an animation "
                    "with an empty name.\n",
                    characterId.c_str()
                );

                characterIsValid = false;
                break;
            }

            if (
                animationNames.find(animationName) !=
                animationNames.end()
            )
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Character '%s' has duplicate "
                    "animation: %s\n",
                    characterId.c_str(),
                    animationName.c_str()
                );

                characterIsValid = false;
                break;
            }

            if (
                !animationJson.contains("frames") ||
                !animationJson["frames"].is_array()
            )
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Animation '%s' of character '%s' "
                    "has no valid frames array.\n",
                    animationName.c_str(),
                    characterId.c_str()
                );

                characterIsValid = false;
                break;
            }

            const nlohmann::json& frameArray =
                animationJson["frames"];

            if (frameArray.empty())
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Animation '%s' of character '%s' "
                    "has no frames.\n",
                    animationName.c_str(),
                    characterId.c_str()
                );

                characterIsValid = false;
                break;
            }

            std::vector<std::string> frames;
            frames.reserve(frameArray.size());

            for (
                std::size_t frameIndex = 0;
                frameIndex < frameArray.size();
                ++frameIndex
            )
            {
                if (!frameArray[frameIndex].is_string())
                {
                    std::printf(
                        "[CharacterDatabase] "
                        "Frame %zu of animation '%s' "
                        "in character '%s' "
                        "must be a string.\n",
                        frameIndex,
                        animationName.c_str(),
                        characterId.c_str()
                    );

                    characterIsValid = false;
                    break;
                }

                const std::string frameId =
                    frameArray[frameIndex]
                        .get<std::string>();

                if (frameId.empty())
                {
                    std::printf(
                        "[CharacterDatabase] "
                        "Animation '%s' of character '%s' "
                        "contains an empty frame ID.\n",
                        animationName.c_str(),
                        characterId.c_str()
                    );
                    

                    characterIsValid = false;
                    break;
                }

                if (!TextureDatabase::Contains(frameId))
                {
                    std::printf(
                        "[CharacterDatabase] "
                        "Animation '%s' of character '%s'"
                        "references unknown texture: %s\n",
                        animationName.c_str(),
                        characterId.c_str(),
                        frameId.c_str()
                    );

                    characterIsValid = false;
                    break;
                }

                frames.push_back(frameId);
            }

            if (!characterIsValid)
            {
                break;
            }

            if (
                !animationJson.contains("frameDelay") ||
                !animationJson[
                    "frameDelay"
                ].is_number_integer()
            )
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Animation '%s' of character '%s' "
                    "has no valid integer frameDelay.\n",
                    animationName.c_str(),
                    characterId.c_str()
                );

                characterIsValid = false;
                break;
            }

            const int frameDelay =
                animationJson["frameDelay"]
                    .get<int>();

            if (frameDelay <= 0)
            {
                std::printf(
                    "[CharacterDatabase] "
                    "Animation '%s' of character '%s' "
                    "has invalid frameDelay: %d\n",
                    animationName.c_str(),
                    characterId.c_str(),
                    frameDelay
                );

                characterIsValid = false;
                break;
            }

            character.AddAnimation(
                animationName,
                frames,
                frameDelay
            );

            animationNames.insert(
                animationName
            );
        }

        if (!characterIsValid)
        {
            ++skippedCount;
            continue;
        }

        if (
            animationNames.find(defaultAnimation) ==
            animationNames.end()
        )
        {
            std::printf(
                "[CharacterDatabase] "
                "Character '%s' references missing "
                "default animation: %s\n",
                characterId.c_str(),
                defaultAnimation.c_str()
            );

            ++skippedCount;
            continue;
        }

        character.SetDefaultAnimation(
            defaultAnimation
        );

        if (!Register(character))
        {
            ++skippedCount;
            continue;
        }

        ++registeredCount;
    }

    std::printf(
        "[CharacterDatabase] JSON load complete: "
        "%zu registered, %zu skipped.\n",
        registeredCount,
        skippedCount
    );

    return registeredCount > 0;
}

bool CharacterDatabase::Register(
    const CharacterData& character
)
{
    if (character.id.empty())
    {
        std::printf(
            "[CharacterDatabase] "
            "Cannot register an empty character ID.\n"
        );

        return false;
    }

    if (character.animations.empty())
    {
        std::printf(
            "[CharacterDatabase] "
            "Character '%s' has no animations.\n",
            character.id.c_str()
        );

        return false;
    }

    if (
        character.animations.find(
            character.defaultAnimation
        ) == character.animations.end()
    )
    {
        std::printf(
            "[CharacterDatabase] "
            "Character '%s' has an invalid "
            "default animation: %s\n",
            character.id.c_str(),
            character.defaultAnimation.c_str()
        );

        return false;
    }

    if (Contains(character.id))
    {
        std::printf(
            "[CharacterDatabase] "
            "Duplicate character ID: %s\n",
            character.id.c_str()
        );

        return false;
    }

    characters.emplace(
        character.id,
        character
    );

    std::printf(
        "[CharacterDatabase] Registered: "
        "%s (%zu animation(s))\n",
        character.id.c_str(),
        character.animations.size()
    );

    return true;
}

bool CharacterDatabase::Contains(
    const std::string& id
)
{
    return
        characters.find(id) !=
        characters.end();
}

CharacterData CharacterDatabase::Get(
    const std::string& id
)
{
    const auto found =
        characters.find(id);

    if (found == characters.end())
    {
        std::printf(
            "[CharacterDatabase] "
            "Character not found: %s\n",
            id.c_str()
        );

        CharacterData empty;
        empty.id = "none";

        return empty;
    }

    return found->second;
}

std::size_t CharacterDatabase::GetCount()
{
    return characters.size();
}

void CharacterDatabase::Clear()
{
    characters.clear();
}