#pragma once

#include "Item.h"
#include "InventorySlot.h"
#include <vector>

class EventManager;

class Inventory {
public:
    void SetEventManager(EventManager* eventManager);

    void Clear();
    void AddItem(const Item& item);
    bool HasItem(const std::string& id);
    bool RemoveItem(const std::string& id);
    void RestoreItem(const Item& item, int count);
    const InventorySlot* GetSlot(const std::string& id) const;
    const std::vector<InventorySlot>& GetInventory() const;

private:
    std::vector<InventorySlot> slots;
    EventManager* eventManager = nullptr;
};