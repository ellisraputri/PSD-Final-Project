#ifndef ZOORK_BUFFCHARACTERITEMCOMMAND_H
#define ZOORK_BUFFCHARACTERITEMCOMMAND_H

#include "ItemUseCommand.h"
#include "Item/Item.h"
#include <utility>

class BuffCharacterItemCommand : public ItemUseCommand {
public:
    explicit BuffCharacterItemCommand(GameObject* g1, Item* required): 
        ItemUseCommand(g1, required) {}

    void execute() override;
};


#endif //ZOORK_BUFFCHARACTERITEMCOMMAND_H
