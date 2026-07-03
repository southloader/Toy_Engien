#pragma once

#include <string>
#include <unordered_map>

#include "Quest.h"

class QuestDatabase {
public: 
    static void Init();

    static Quest Get(const std::string& id);

private:
    static std::unordered_map<std::string, Quest> quests;
};

