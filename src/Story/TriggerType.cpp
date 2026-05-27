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

    throw std::runtime_error("Unknown trigger type: " + str);
}