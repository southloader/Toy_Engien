#include "Inventory.h"
#include <cstdio>

void Inventory::AddItem(const Item& item){
    for(auto& slot : slots) {
        if(slot.item.id == item.id && slot.count < slot.item.maxStack) {
            slot.count++;
            printf("%s x%d\n", slot.item.name.c_str(), slot.count);
            return;
        }
    }

    InventorySlot slot;
    slot.item = item;
    slot.count = 1;

    slots.push_back(slot);

    printf("%s x1\n", item.name.c_str());
}

bool Inventory::HasItem(const std::string& id) {
    for (auto& slot : slots) {
        if (slot.item.id == id) {
            printf("%s x%d\n", slot.item.name.c_str(), slot.count);
            return true;
        }
    }
    return false;
}

bool Inventory::RemoveItem(const std::string& id) {
    for (auto it = slots.begin(); it != slots.end(); ++it) {
        if (it->item.id == id) {

            it->count--;

            if (it->count <= 0) {
                printf("Item removed: %s\n", it->item.name.c_str());
                slots.erase(it);
            }
            else {
                printf("Item decreased: %s x%d\n", it->item.name.c_str(), it->count);
            }

            return true;
        }
    }

    printf("Item not found: %s\n", id.c_str());
    return false;
}
const InventorySlot* Inventory::GetSlot(const std::string& id) const {
    for (auto& slot : slots) {
        if (slot.item.id == id) {
            return &slot;
        }
    }
    return nullptr;
};

const std::vector<InventorySlot>& Inventory::GetInventory() const {
    return slots;
}