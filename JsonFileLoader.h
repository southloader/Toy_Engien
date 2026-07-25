#pragma once

#include <string>

#include <nlohmann/json.hpp>

class JsonFileLoader
{
public:
    static bool Load(
        const std::string& filePath,
        nlohmann::json& outputJson
    );
};