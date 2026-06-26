#pragma once
#include <string>

struct Item {
    std::string id;
    std::string name;
    std::string description;

    int maxStack = 1;
};
