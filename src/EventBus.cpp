#include "EventBus.h"

EventBus* EventBus::busInstance = nullptr;

EventBus* EventBus::instance() {
    if (!busInstance) {
        busInstance = new EventBus();
    }
    return busInstance;
}

void EventBus::subscribe(Listener listener) {
    listeners.push_back(listener);
}

void EventBus::emit(const Event& event) {
    for (auto& listener : listeners) {
        listener(event);
    }
}