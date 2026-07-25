#include "QuestDatabase.h"
#include <cstdio>

std::unordered_map<std::string, Quest> QuestDatabase::quests;

void QuestDatabase::Init() {
    Quest collectPotion;

    collectPotion.id = "collect_potion";
    collectPotion.title = "Potion Collector";
    collectPotion.description = "Collect 3 potions.";

    collectPotion.state = QuestState::Available;
    collectPotion.objectiveType = QuestObjectiveType::CollectItem;
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

    Quest killSlime;

    killSlime.id = "kill_slime";
    killSlime.title = "Slime Hunter";
    killSlime.description = "Defeat 2 slime.";

    killSlime.state = QuestState::Available;
    killSlime.objectiveType = QuestObjectiveType::DefeatTarget;

    // 처치 퀘스트는 아이템을 소비하지 않는다.
    killSlime.consumeTargetItem = false;

    // CombatSystem의 EnemyKilled 이벤트 subjectId와 같아야 한다.
    killSlime.targetId = "slime";
    killSlime.targetCount = 2;
    killSlime.currentCount = 0;

    killSlime.rewardItemId = "potion";
    killSlime.rewardAmount = 1;
    killSlime.rewardGold = 50;

    killSlime.offerDialogue =
        "마을 근처에 슬라임이 나타났네. 처치해 주겠나?";

    killSlime.acceptDialogue =
    {
        "고맙네.",
        "슬라임 두 마리를 처치하고 돌아오게."
    };

    killSlime.progressDialogue =
    {
        "아직 슬라임을 처치하지 못했군.",
        "마을 근처를 다시 살펴보게."
    };

    killSlime.completeDialogue =
    {
        "슬라임을 처치했군!",
        "약속한 보상을 주겠네."
    };

    killSlime.alreadyCompletedDialogue =
    {
        "덕분에 마을이 한동안 조용할 것 같네.",
        "수고했네."
    };
    quests[killSlime.id] = killSlime;
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

const std::unordered_map<std::string, Quest>& QuestDatabase::GetAll() {
    return quests;
}