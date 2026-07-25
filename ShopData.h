#pragma once

#include <string>
#include <vector>

struct ShopEntry
{
    std::string itemId;
    int price = 0;
};

struct ShopData
{
    std::string id;
    std::string name;

    std::vector<ShopEntry> entries;
};