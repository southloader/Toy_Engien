#pragma once

#include <string>
#include <vector>

enum class QuestState {
    Available,
    Active,
    Completed
};

enum class QuestType {
    CollectItem,
    KillMonster,
    TalkToNPC
};

enum class QuestResult {
    Success,
    ConditionNotMet,
    AlreadyCompleted,
    QuestNotFound
};

struct Quest {
    std::string id;
    std::string title;
    std::string description;

    QuestState state = QuestState::Available;
    QuestType type;
    std::string targetId;
    int targetCount = 0;
    int currentCount = 0;

    std::string rewardItemId;
    int rewardAmount = 1;
    int rewardGold = 0;

    std::vector<std::string> acceptDialogue;
    std::vector<std::string> progressDialogue;
    std::vector<std::string> completeDialogue;
    std::vector<std::string> alreadyCompletedDialogue;
};