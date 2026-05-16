#include "PassageDefaultUnlockCommand.h"
#include <iostream>
#include "Passage/Passage.h"
#include "Character/Player.h"

void PassageDefaultUnlockCommand::execute() {
    Passage* passage1 = static_cast<Passage*>(gameObject);
    Passage* passage2 = static_cast<Passage*>(otherPassage);

    if (usedItem == requiredItem) {
        passage1->setLocked(false);
        passage2->setLocked(false);
        usedItem->setIsUsed(true);
        std::cout << "You unlocked the passage.\n";
        return;
    }

    std::cout << "That item cannot unlock this passage.\n";
}