#include "JsonFileLoader.h"

#include <cstdio>
#include <fstream>

bool JsonFileLoader::Load(
    const std::string& filePath,
    nlohmann::json& outputJson
)
{
    outputJson = nlohmann::json();

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::printf(
            "[JsonFileLoader] "
            "Could not open file: %s\n",
            filePath.c_str()
        );

        return false;
    }

    try
    {
        file >> outputJson;
    }
    catch (
        const nlohmann::json::parse_error& error
    )
    {
        std::printf(
            "[JsonFileLoader] "
            "Parse error in %s\n"
            "Message: %s\n"
            "Byte position: %zu\n",
            filePath.c_str(),
            error.what(),
            error.byte
        );

        outputJson = nlohmann::json();

        return false;
    }
    catch (const std::exception& error)
    {
        std::printf(
            "[JsonFileLoader] "
            "Failed to load %s\n"
            "Message: %s\n",
            filePath.c_str(),
            error.what()
        );

        outputJson = nlohmann::json();

        return false;
    }

    if (outputJson.is_discarded())
    {
        std::printf(
            "[JsonFileLoader] "
            "Invalid JSON document: %s\n",
            filePath.c_str()
        );

        outputJson = nlohmann::json();

        return false;
    }

    std::printf(
        "[JsonFileLoader] Loaded: %s\n",
        filePath.c_str()
    );

    return true;
}