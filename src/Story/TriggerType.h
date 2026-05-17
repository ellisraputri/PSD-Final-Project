#ifndef ZOORK_TRIGGERTYPE_H
#define ZOORK_TRIGGERTYPE_H

#include <string>
#include <stdexcept>

enum class TriggerType {
    ENTER_ROOM,
    USE_ITEM,
    TAKE_ITEM,
    ATTACK_ENEMY
};

TriggerType stringToTriggerType(const std::string& str);

#endif