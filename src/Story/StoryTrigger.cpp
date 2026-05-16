#include "StoryTrigger.h"

bool StoryTrigger::matches(TriggerType t, const std::string& target) const {
    return (triggerType == t && triggerTarget == target);
}

std::string StoryTrigger::getResultFlag() const {
    return resultFlag;
}

std::string StoryTrigger::getTriggerPrint() const {
    return resultPrint;
}