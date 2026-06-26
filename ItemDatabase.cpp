#include "ItemDatabase.h"
#include <cstdio>

std::unordered_map<std::string, Item> ItemDatabase::items;

void ItemDatabase::Init() {
    Item potion;
    potion.id = "potion";
    potion.name = "Potion";
    potion.maxStack = 99;
    potion.description = "Restore HP.";

    items[potion.id] = potion;

    Item oldSword;
    oldSword.id = "old_sword";
    oldSword.name = "Old Sword";
    oldSword.maxStack = 1;
    oldSword.description = "A worn old sword.";

    items[oldSword.id] = oldSword;
}

Item ItemDatabase::Get(const std::string& id) {
    if (items.find(id) == items.end()) {
        printf("Item not found: %s\n", id.c_str());

        Item empty;
        empty.id = "none";
        empty.name = "Name";
        empty.description = "Invalid item.";

        return empty;
    }

    return items[id];
}