#include "TriggerType.h"

TriggerType stringToTriggerType(const std::string& str) {
    if (str == "ENTER_ROOM") {
        return TriggerType::ENTER_ROOM;
    }

    if (str == "TALK_CHARACTER") {
        return TriggerType::TALK_CHARACTER;
    }

    if (str == "TAKE_ITEM") {
        return TriggerType::TAKE_ITEM;
    }

    if (str == "WIN_COMBAT") {
        return TriggerType::WIN_COMBAT;
    }

    if (str == "UNLOCK_MECHANISM"){
        return TriggerType::UNLOCK_MECHANISM;
    }

    throw std::runtime_error("Unknown trigger type: " + str);
}