#include "EventManager.h"

void EventManager::Subscribe(EventType type, const Listener& listener){
    listeners[type].push_back(listener);
}

void EventManager::Emit(const Event& event){
    auto found = listeners.find(event.type);

    if (found == listeners.end()) {
        return;
    }

    for (const auto& listener : found->second) {
        listener(event);
    }
}