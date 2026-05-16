#ifndef ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H
#define ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H

#include "ItemUseCommand.h"
#include "Item/Item.h"
#include <utility>

class PassageDefaultUnlockCommand : public ItemUseCommand {
public:
    explicit PassageDefaultUnlockCommand(GameObject* g, Item* required): 
        ItemUseCommand(g, required) {}

    void execute() override;
};


#endif //ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H
