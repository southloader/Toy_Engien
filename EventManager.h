#pragma once

#include <functional>
#include <map>
#include <vector>

#include "Event.h"

class EventManager {
public:
    using Listener = std::function<void(const Event&)>;
    using ListenerId = unsigned int;

    ListenerId Subscribe(EventType type, const Listener& listener);

    bool Unsubscribe(EventType type, ListenerId listenerId);

    void Emit(const Event& event);

private:
    struct ListenerEntry {
        ListenerId id;
        Listener callback;
    };
    std::map<EventType, std::vector<ListenerEntry>> listeners;

    ListenerId nextListenerId = 1;
};