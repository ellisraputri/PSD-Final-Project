#include "PassageDefaultUnlockCommand.h"
#include <iostream>
#include "Passage/Passage.h"
#include "Character/Player.h"

void PassageDefaultUnlockCommand::execute() {
    Passage* passage1 = static_cast<Passage*>(gameObject);
    Passage* passage2 = static_cast<Passage*>(otherPassage);
    Room* currentRoom = Player::instance()->getCurrentRoom();

    if (currentRoom->isPassageExist(passage1->getName()) || currentRoom->isPassageExist(passage2->getName())){
        if (usedItem == requiredItem) {
            passage1->setLocked(false);
            passage2->setLocked(false);
            usedItem->setIsUsed(true);
            std::cout << "You unlocked the passage.\n";
            return;
        }
    }

    std::cout << "That item cannot be used right now.\n";
}