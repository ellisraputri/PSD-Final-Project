#include "MechanismUnlockCommand.h"
#include <iostream>
#include "Mechanism/Mechanism.h"
#include "Character/Player.h"

void MechanismUnlockCommand::execute() {
    Mechanism* mechanism = static_cast<Mechanism*>(gameObject);
    Room* currentRoom = Player::instance()->getCurrentRoom();

    if (currentRoom->isMechanismExist(mechanism->getName())){
        if (usedItem == requiredItem) {
            mechanism->setLocked(false);
            usedItem->setIsUsed(true);
            std::cout << mechanism->getResultPrint() << std::endl;

            EventBus::instance()->emit({
                TriggerType::UNLOCK_MECHANISM,
                mechanism->getName()
            });

            return;
        }
    }

    std::cout << "That item cannot be used right now.\n";
}