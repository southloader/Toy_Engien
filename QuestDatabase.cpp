#include "QuestDatabase.h"
#include <cstdio>

std::unordered_map<std::string, Quest> QuestDatabase::quests;

void QuestDatabase::Init() {
    Quest collectPotion;

    collectPotion.id = "collect_potion";
    collectPotion.title = "Potion Collector";
    collectPotion.description = "Collect 3 potions.";

    collectPotion.state = QuestState::Available;
    collectPotion.type = QuestType::CollectItem;
    collectPotion.consumeTargetItem = true;

    collectPotion.targetId = "potion";
    collectPotion.targetCount = 3;
    collectPotion.currentCount = 0;

    collectPotion.rewardItemId = "old_sword";
    collectPotion.rewardAmount = 1;
    collectPotion.rewardGold = 100;

    collectPotion.acceptDialogue = {
        "좋아요.",
        "포션 3개를 가져와 주세요."
    };

    collectPotion.progressDialogue = {
        "아직 포션이 부족해요.",
        "포션 3개가 필요합니다."
    };

    collectPotion.completeDialogue = {
        "좋아요!",
        "여기 보수입니다."
    };

    collectPotion.alreadyCompletedDialogue = {
        "이미 도와주셨어요.",
        "다시 한 번 고마워요."
    };

    quests[collectPotion.id] = collectPotion;
}

Quest QuestDatabase::Get(const std::string& id) {
    if (quests.find(id) == quests.end()) {
        printf("Quest not found: %s\n", id.c_str());

        Quest empty;
        empty.id = "none";
        empty.title = "None";
        empty.description = "Invalid quest.";

        return empty;
    }

    return quests[id];
}