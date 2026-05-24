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

void StoryTrigger::setTriggerDone(bool done) {
    triggerDone = done;
}

bool StoryTrigger::isTriggerDone() const {
    return triggerDone;
}

std::string StoryTrigger::getFlagDone() const {
    return flagDone;
}

std::string StoryTrigger::getFlagCondition() const {
    return flagCondition;
}

bool StoryTrigger::isLockPlayer() const {
    return lockPlayer;
}