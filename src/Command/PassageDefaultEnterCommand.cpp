

#include "Passage/Passage.h"
#include "PassageDefaultEnterCommand.h"

void PassageDefaultEnterCommand::execute() {
    static_cast<Passage*>(gameObject)->getTo()->enter();
}