#include "QuestLog.h"
#include <cstdio>

void QuestLog::Clear() {quests.clear();};

void QuestLog::AddQuest(const Quest& quest){
    quests.push_back(quest);
    printf("Quest Added : %s\n", quest.title.c_str());
}

bool QuestLog::HasQuest(const std::string& id) const {
    for (const auto& quest : quests) {
        if (quest.id == id) {
            return true;
        }
    }

    return false;
}

bool QuestLog::RemoveQuest(const std::string& id) {
    for (auto it = quests.begin(); it != quests.end(); ++it) {
        if (it->id == id) {
            quests.erase(it);
            return true;
        }
    }

    return false;
}

Quest* QuestLog::GetQuest(const std::string& id) {
    for (auto& quest : quests) {
        if (quest.id == id) {
            return &quest;
        }
    }
    return nullptr;
}

const Quest* QuestLog::GetQuest(const std::string& id) const {
    for (const auto& quest : quests) {
        if (quest.id == id) {
            return &quest;
        }
    }
    return nullptr;
}

const std::vector<Quest>& QuestLog::GetQuests() const {
    return quests;    
}

std::vector<Quest>& QuestLog::GetMutableQuests() {
    return quests;
}