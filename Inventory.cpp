#include "Inventory.h"
#include "EventManager.h"
#include <cstdio>

void Inventory::SetEventManager(EventManager* eventManager) {
    this->eventManager = eventManager;
}

void Inventory::Clear() { slots.clear();};

void Inventory::AddItem(const Item& item){
    for(auto& slot : slots) {
        if(slot.item.id == item.id && slot.count < slot.item.maxStack) {
            slot.count++;
            if (eventManager != nullptr) {
                eventManager->Emit({EventType::ItemAdded,item.id,1});
            }            
            return;
        }
    }

    InventorySlot slot;
    slot.item = item;
    slot.count = 1;

    slots.push_back(slot);

    if (eventManager != nullptr) {
        eventManager->Emit({EventType::ItemAdded,item.id,1});
    } 
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
            if (eventManager != nullptr) {
                eventManager->Emit({EventType::ItemRemoved, id , 1});
            }

            return true;
        }
    }

    return false;
}

void Inventory::RestoreItem(const Item& item, int count) {
    if (count <= 0) {
        return;
    }

    InventorySlot slot;
    slot.item = item;
    slot.count = count;

    slots.push_back(slot);
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