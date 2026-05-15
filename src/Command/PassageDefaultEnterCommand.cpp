#include "Passage/Passage.h"
#include "PassageDefaultEnterCommand.h"

void PassageDefaultEnterCommand::execute() {
    Passage* passage = static_cast<Passage*>(gameObject);

    if (passage->isLocked()) {
        std::cout << "The way is locked.\n";
        return;
    }

    passage->getTo()->enter();
}