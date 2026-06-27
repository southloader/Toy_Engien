#include "QuestLog.h"
#include <cstdio>

void QuestLog::AddQuest(const Quest& quest){
    quests.push_back(quest);
    printf("Quest Added : %s\n", quest.title.c_str());
}

bool QuestLog::HasQuest(const std::string& id) {
    for(auto& q : quests) {
        if(q.id == id) {
            return true;
        }
    }
    printf("Quest : %s Not Found\n", id);
    return false;
}

Quest* QuestLog::GetQuest(const std::string& id) {
    for(auto& q : quests) {
        if(q.id == id){
            return &q;
        }
    }
    printf("Quest : %s Not Found\n", id);
    return nullptr;
}

const std::vector<Quest>& QuestLog::GetQuests() const {
    return quests;    
}