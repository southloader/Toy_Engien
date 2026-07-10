#pragma once

#include <functional>
#include <map>
#include <vector>

#include "Event.h"

class EventManager {
public:
    using Listener = std::function<void(const Event&)>;

    void Subscribe(EventType type, const Listener& listener);

    void Emit(const Event& event);

private:
    std::map<EventType, std::vector<Listener>> listeners;
};