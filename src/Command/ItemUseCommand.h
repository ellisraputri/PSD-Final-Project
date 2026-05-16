#ifndef ZOORK_ITEMUSECOMMAND_H
#define ZOORK_ITEMUSECOMMAND_H

#include "Command.h"
#include "Item/Item.h"
#include <utility>

class ItemUseCommand : public Command {
public:
    explicit ItemUseCommand(GameObject* g, Item* required): 
        Command(g),
        requiredItem(required),
        usedItem(nullptr) {}

    void setUsedItem(Item* item);

    virtual void execute() override = 0;

protected:
    Item* requiredItem;
    Item* usedItem;
};


#endif //ZOORK_ITEMUSECOMMAND_H
