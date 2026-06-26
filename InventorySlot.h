#pragma once

#include "Item.h"

struct InventorySlot
{
    Item item;
    int count = 0;
    bool equipped = false;
};