#include "StoryManager.h"
#include <iostream>

StoryManager* StoryManager::storyInstance = nullptr;

StoryManager::StoryManager() {
    EventBus::instance()->subscribe(
        [this](const Event& event) {
            trigger(event.type, event.target);
        }
    );
}

StoryManager* StoryManager::instance() {
    if (!storyInstance) {
        storyInstance = new StoryManager();
    }
    return storyInstance;
}

void StoryManager::addTrigger(const StoryTrigger& trigger) {
    triggers.push_back(trigger);
}

bool StoryManager::hasFlag(const std::string& flag) const {
    return flags.find(flag) != flags.end();
}

void StoryManager::trigger(TriggerType type, const std::string& target) {
    Player* player = Player::instance();

    for (auto& trigger : triggers) {
        if (trigger.isTriggerDone() || !trigger.matches(type, target)) {
            continue;
        }
        if ((trigger.getFlagCondition() != "" && !hasFlag(trigger.getFlagCondition()))) {
            continue;
        }

        flags.insert(trigger.getResultFlag());
        triggerAdditionalAction(trigger);

        player->setLocked(trigger.isLockPlayer());
        std::cout << "\n" << trigger.getTriggerPrint() << "\n";

        if (trigger.getFlagDone() == "" || 
            (trigger.getFlagDone() != "" && hasFlag(trigger.getFlagDone()))){

            trigger.setTriggerDone(true);
            if(player->isLocked()) player->setLocked(false);
        }
    }
}

void StoryManager::triggerAdditionalAction(StoryTrigger& trigger) {
    Information* info = Information::instance();
    Player* player = Player::instance();

    std::string name = trigger.getResultFlag();

    if (name == "took_black_pearl") {
        std::shared_ptr<Room> hall = info->getRoom("castlecenter-hall");
        std::shared_ptr<Room> throne = info->getRoom("castlecenter-throne");
        std::shared_ptr<Character> character = info->getCharacter("king iotate");
        std::shared_ptr<Item> item = info->getItem("king dead body");

        hall->removeCharacter(character);
        throne->addItem(item);
        info->setRoom("castlecenter-hall", hall);
        info->setRoom("castlecenter-thone", throne);
    }

    if (name == "entered_corridor1" || name == "entered_library_entrace") {
        player->setCheckpoint(trigger.getTriggerTarget());
    }
}