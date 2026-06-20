#include "Inventory.h"
#include <cstdio>

void Inventory::AddItem(const std::string& itemName){
    items.push_back(itemName);

    printf("Item added: %s\n", itemName.c_str());
}

bool Inventory::HasItem(const std::string& itemName){
    for (auto& item : items) {
        if (item == itemName) {
            return true;
        }
    }
    return false;
}

void Inventory::RemoveItem(const std::string& itemName) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (*it == itemName) {
            items.erase(it);
            printf("Item removed: %s\n", itemName.c_str());
            return;
        }
    }
}

void Inventory::PrintItems() {
    printf("Inventory:\n");
    for (auto& item : items){
        printf("- %s\n", item.c_str());
    }
}