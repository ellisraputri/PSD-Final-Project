#ifndef ZOORK_MECHANISMUNLOCKCOMMAND_H
#define ZOORK_MECHANISMUNLOCKCOMMAND_H

#include "ItemUseCommand.h"
#include "Item/Item.h"
#include <utility>

class MechanismUnlockCommand : public ItemUseCommand {
public:
    explicit MechanismUnlockCommand(GameObject* g1, Item* required): 
        ItemUseCommand(g1, required) {}

    void execute() override;
};


#endif //ZOORK_MECHANISMUNLOCKCOMMAND_H
