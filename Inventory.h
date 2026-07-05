#pragma once

#include "Item.h"
#include "InventorySlot.h"
#include <vector>

class Inventory {
public:
    void Clear();
    void AddItem(const Item& item);
    bool HasItem(const std::string& id);
    bool RemoveItem(const std::string& id);
    const InventorySlot* GetSlot(const std::string& id) const;

    const std::vector<InventorySlot>& GetInventory() const;

private:
    std::vector<InventorySlot> slots;
};