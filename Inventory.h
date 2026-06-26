#pragma once

#include "Item.h"
#include "InventorySlot.h"
#include <vector>

class Inventory {
public:
    void AddItem(const Item& item);
    bool HasItem(const std::string& id);
    bool RemoveItem(const std::string& id);

    const std::vector<InventorySlot>& GetInventory() const;

private:
    std::vector<InventorySlot> slots;
};