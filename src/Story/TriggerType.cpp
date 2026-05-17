#include "TriggerType.h"

TriggerType stringToTriggerType(const std::string& str) {
    if (str == "ENTER_ROOM") {
        return TriggerType::ENTER_ROOM;
    }

    if (str == "TALK_CHARACTER") {
        return TriggerType::TALK_CHARACTER;
    }

    throw std::runtime_error("Unknown trigger type: " + str);
}