#include "StoryTrigger.h"

bool StoryTrigger::matches(TriggerType t, const std::string& target) const {
    return (triggerType == t && triggerTarget == target);
}

std::string StoryTrigger::getResult() const {
    return resultFlag;
}