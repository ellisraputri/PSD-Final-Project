#include "TriggerType.h"

TriggerType stringToTriggerType(const std::string& str) {
    if (str == "ENTER_ROOM") {
        return TriggerType::ENTER_ROOM;
    }

    if (str == "TAKE_ITEM") {
        return TriggerType::TAKE_ITEM;
    }

    if (str == "USE_ITEM") {
        return TriggerType::USE_ITEM;
    }

    throw std::runtime_error("Unknown trigger type: " + str);
}