#include "EventManager.h"

#include <algorithm>

EventManager::ListenerId EventManager::Subscribe(EventType type, const Listener& listener){
    ListenerId id = nextListenerId++;
    listeners[type].push_back({id, listener});

    return id;
}

bool EventManager::Unsubscribe(EventType type, ListenerId listenerId){
    auto found = listeners.find(type);
    if (found == listeners.end()){
        return false;
    }
    auto& entries = found->second;

    auto newEnd = std::remove_if(
        entries.begin(), 
        entries.end(), 
        [listenerId](const ListenerEntry& entry){
            return entry.id == listenerId;
        }
    );

    if (newEnd == entries.end()) {
        return false;
    }

    entries.erase(newEnd, entries.end());

    if (entries.empty()) {
        listeners.erase(found);
    }
    return true;
}

void EventManager::Emit(const Event& event){
    auto found = listeners.find(event.type);

    if (found == listeners.end()) {
        return;
    }

    const auto entries = found->second;

    for (const auto& entry : entries) {
        entry.callback(event);
    }
}