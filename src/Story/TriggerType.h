#ifndef ZOORK_TRIGGERTYPE_H
#define ZOORK_TRIGGERTYPE_H

#include <string>
#include <stdexcept>

enum class TriggerType {
    ENTER_ROOM,
    TALK_CHARACTER,
    TAKE_ITEM, 
    WIN_COMBAT,
    UNLOCK_MECHANISM
};

TriggerType stringToTriggerType(const std::string& str);

#endif