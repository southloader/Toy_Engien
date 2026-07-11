#pragma once

#include <string>

enum class EventType {
    // Inventory
    ItemAdded,
    ItemRemoved,

    // Gold
    GoldChanged,

    // Quest
    QuestAccepted,
    QuestCompleted,
    QuestAbandoned,

    // Combat (Future)
    EnemyKilled,
    PlayerDamaged,
    PlayerDead,

    // Scene (Future)
    SceneChanged,

    // Save (Future)
    GameSaved,
    GameLoaded
};

struct Event {
    EventType type;

    std::string subjectId;

    int amount = 0;
};