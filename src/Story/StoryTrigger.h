#ifndef ZOORK_STORYTRIGGER_H
#define ZOORK_STORYTRIGGER_H

#include "TriggerType.h"
#include <string>

class StoryTrigger {
public:
    StoryTrigger(TriggerType t, const std::string& target, const std::string& result):
        triggerType(t), triggerTarget(target), resultFlag(result){}

    bool matches(TriggerType t, const std::string& target) const;

    std::string getResult() const;

private:
    TriggerType triggerType;

    std::string triggerTarget;

    std::string resultFlag;
};

#endif