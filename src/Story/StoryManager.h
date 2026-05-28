#ifndef ZOORK_STORYMANAGER_H
#define ZOORK_STORYMANAGER_H

#include "StoryTrigger.h"
#include "Character/Player.h"
#include "EventBus.h"
#include "Information/Information.h"
#include <set>
#include <vector>

class StoryManager {
public:
    static StoryManager* instance();

    void addTrigger(const StoryTrigger&);

    void triggerAdditionalAction(StoryTrigger&);
    void trigger(TriggerType, const std::string&);

    bool hasFlag(const std::string&) const;

    void editItemInRoom(std::string, std::string, bool);
    void editCharacterInRoom(std::string, std::string, bool);

private:
    static StoryManager* storyInstance;

    std::vector<StoryTrigger> triggers;

    std::set<std::string> flags;

    StoryManager();
};

#endif