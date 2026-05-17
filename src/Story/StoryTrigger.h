#ifndef ZOORK_STORYTRIGGER_H
#define ZOORK_STORYTRIGGER_H

#include "TriggerType.h"
#include <string>

class StoryTrigger {
public:
    StoryTrigger(TriggerType t, const std::string& target, const std::string& flag, const std::string& res, const std::string& done):
        triggerType(t), triggerTarget(target), resultFlag(flag), resultPrint(res), flagDone(done){}

    bool matches(TriggerType t, const std::string& target) const;

    std::string getResultFlag() const;

    std::string getTriggerPrint() const;

    void setTriggerDone(bool);
    bool isTriggerDone() const;

    std::string getFlagDone() const;

private:
    TriggerType triggerType;

    std::string triggerTarget;

    std::string resultFlag;

    std::string resultPrint;

    bool triggerDone = false;

    std::string flagDone;
};

#endif