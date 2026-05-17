#include "StoryManager.h"
#include <iostream>

StoryManager* StoryManager::storyInstance = nullptr;

StoryManager* StoryManager::instance() {
    if (!storyInstance) {
        storyInstance = new StoryManager();
    }
    return storyInstance;
}

void StoryManager::addTrigger(const StoryTrigger& trigger) {
    triggers.push_back(trigger);
}

void StoryManager::trigger(TriggerType type, const std::string& target) {
    for (auto& trigger : triggers) {
        if (trigger.isTriggerDone() || !trigger.matches(type, target)) {
            continue;
        }

        flags.insert(trigger.getResultFlag());
        std::cout << trigger.getTriggerPrint() << std::endl;

        if (trigger.getFlagDone() != "" && hasFlag(trigger.getFlagDone())){
            trigger.setTriggerDone(true);
        }
    }
}

bool StoryManager::hasFlag(const std::string& flag) const {
    return flags.find(flag) != flags.end();
}