#pragma once

#include <string>
#include <vector>

enum class QuestState {
    Available,
    Active,
    Completed
};

// 퀘스트가 어떤 종류의 외부 사건을 목표로 하는지 나타낸다.
// 사건을 발생시키는 시스템의 구현 방식은 알지 않는다.
enum class QuestObjectiveType {
    CollectItem,
    DefeatTarget,
    TalkToTarget,
    ReachArea,
    Interact,
    Custom
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
    QuestObjectiveType objectiveType = QuestObjectiveType::Custom;

    // 목표 대상의 범용 ID
    //
    // 예:
    // CollectItem  -> "potion"
    // DefeatTarget -> "slime"
    // TalkToTarget -> "village_chief"
    // ReachArea    -> "ancient_forest"
    // Interact     -> "sealed_door"
    std::string targetId;

    int targetCount = 0;
    int currentCount = 0;

    // CollectItem 목표를 완료할 때
    // 실제 아이템을 차감할지 여부
    bool consumeTargetItem = true;

    std::string rewardItemId;

    int rewardAmount = 1;
    int rewardGold = 0;

    std::string offerDialogue;
    std::vector<std::string> acceptDialogue;
    std::vector<std::string> progressDialogue;
    std::vector<std::string> completeDialogue;
    std::vector<std::string> alreadyCompletedDialogue;
};