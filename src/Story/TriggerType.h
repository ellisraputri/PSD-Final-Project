#ifndef ZOORK_TRIGGERTYPE_H
#define ZOORK_TRIGGERTYPE_H

#include <string>
#include <stdexcept>

enum class TriggerType {
    ENTER_ROOM,
    TALK_CHARACTER,
};

TriggerType stringToTriggerType(const std::string& str);

#endif