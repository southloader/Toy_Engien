#pragma once

#include <string>
#include <unordered_map>
#include "Quest.h"

class QuestDatabase {
public: 
    static void Init();

    static Quest Get(const std::string& id);

    static const std::unordered_map<std::string, Quest>& GetAll();
    
private:
    static std::unordered_map<std::string, Quest> quests;
};

