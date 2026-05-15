#include "PassageDefaultUnlockCommand.h"
#include <iostream>
#include "Passage/Passage.h"
#include "Character/Player.h"

void PassageDefaultUnlockCommand::execute() {
    Passage* passage = static_cast<Passage*>(gameObject);

    if (usedItem == requiredItem) {
        passage->setLocked(false);
        usedItem->setIsUsed(true);
        std::cout << "You unlocked the passage.\n";
        return;
    }

    std::cout << "That item cannot unlock this passage.\n";
}

void PassageDefaultUnlockCommand::setUsedItem(Item* item) {
    usedItem = item;
}