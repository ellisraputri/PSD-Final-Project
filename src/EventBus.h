#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <functional>
#include <vector>
#include "Story/TriggerType.h"

struct Event {
    TriggerType type;
    std::string target;
};

class EventBus {
public:
    using Listener = std::function<void(const Event&)>;

    static EventBus* instance();

    void subscribe(Listener listener);

    void emit(const Event& event);

private:
    static EventBus* busInstance;

    std::vector<Listener> listeners;
};

#endif