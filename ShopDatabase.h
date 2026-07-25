#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>

#include "ShopData.h"

class ShopDatabase {
public:
    static bool Init(
        const std::string& filePath = 
            "data/shops.json"
    );

    static bool LoadFromJson(
        const std::string& filePath
    );

    static bool Register(
        const ShopData& shop
    );

    static bool Contains(
        const std::string& id
    );

    static const ShopData* Get(
        const std::string& id
    );

    static std::size_t GetCount();

    static void Clear();

private:
    static std::unordered_map<
        std::string,
        ShopData
    > shops;
};