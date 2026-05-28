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
        if (trigger.getFlagDone() != "" && hasFlag(trigger.getFlagDone())) {
            trigger.setTriggerDone(true);
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
        editCharacterInRoom("castlecenter-hall", "king iotate", false);
        editItemInRoom("castlecenter-throne", "king heart", true);
    }

    if (name == "win_combat_guard") {
        std::shared_ptr<Room> bedroom = info->getRoom("library-bedroom");
        player->setCurrentRoom(bedroom.get());
    }

    if (name == "entered_library_jail"){
        std::shared_ptr<Passage> passage = info->getPassage("passage_library-corridor_to_library-mainroom");
        std::shared_ptr<Passage> otherPassage = info->getPassage("passage_library-mainroom_to_library-corridor");
        passage->setLocked(false);
        otherPassage->setLocked(false);
    }

    if (name == "entered_corridor1" || name == "entered_library_entrace" || name == "unlock_mechanism_wood_shelf") {
        player->setCheckpoint(trigger.getTriggerTarget());

        if (name == "unlock_mechanism_wood_shelf"){
            editCharacterInRoom(player->getCurrentRoom()->getName(), "old man qing", false);
            editItemInRoom(player->getCurrentRoom()->getName(), "old man heart", true);
        }
    }
}

void StoryManager::editItemInRoom(std::string roomName, std::string itemName, bool isAdd){
    Information* info = Information::instance();
    std::shared_ptr<Item> item = info->getItem(itemName);
    std::shared_ptr<Room> room = info->getRoom(roomName);
    
    if (isAdd) {
        room->addItem(item);
    }
    else {
        room->removeItem(item);
    }

    info->setRoom(roomName, room);
}

void StoryManager::editCharacterInRoom(std::string roomName, std::string charName, bool isAdd){
    Information* info = Information::instance();
    std::shared_ptr<Character> character = info->getCharacter(charName);
    std::shared_ptr<Room> room = info->getRoom(roomName);
    
    if (isAdd) {
        room->addCharacter(character);
    }
    else {
        room->removeCharacter(character);
    }

    info->setRoom(roomName, room);
}