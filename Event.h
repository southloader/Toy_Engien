#pragma once

#include <string>

enum class EventType {
    ItemAdded,
    ItemRemoved,

    QuestAccepted,
    QuestCompleted,
    QuestAbandoned
};

struct Event {
    EventType type;

    std::string subjectId;

    int amount = 0;
};