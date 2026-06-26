#pragma once

#include <string>
#include <unordered_map>

#include "Item.h"

class ItemDatabase {
public:
    static void Init();
    static Item Get(const std::string& id);

private:
    static std::unordered_map<std::string, Item> items;
};