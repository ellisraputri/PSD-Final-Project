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
    for (const auto& trigger : triggers) {
        if (trigger.matches(type, target)) {
            flags.insert(trigger.getResult());
        }
    }
}

bool StoryManager::hasFlag(const std::string& flag) const {
    return flags.find(flag) != flags.end();
}