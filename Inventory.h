#pragma once

#include <string>
#include <vector>

class Inventory {
public:
    void AddItem(const std::string& itemName);
    bool HasItem(const std::string& itemName);
    void RemoveItem(const std::string& itemName);

    void PrintItems();

private:
    std::vector<std::string> items;

};