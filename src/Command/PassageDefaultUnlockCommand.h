#ifndef ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H
#define ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H

#include "Command.h"
#include "Item/Item.h"
#include <utility>

class PassageDefaultUnlockCommand : public Command {
public:
    explicit PassageDefaultUnlockCommand(GameObject* g, Item* required): 
        Command(g),
        requiredItem(required),
        usedItem(nullptr) {}

    void setUsedItem(Item* item);

    void execute() override;

private:
    Item* requiredItem;
    Item* usedItem;
};


#endif //ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H
