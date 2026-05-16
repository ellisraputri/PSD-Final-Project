#ifndef ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H
#define ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H

#include "ItemUseCommand.h"
#include "Item/Item.h"
#include <utility>

class PassageDefaultUnlockCommand : public ItemUseCommand {
public:
    explicit PassageDefaultUnlockCommand(GameObject* g1, GameObject* g2, Item* required): 
        ItemUseCommand(g1, required), otherPassage(g2) {}

    void execute() override;

private:
    GameObject* otherPassage;
};


#endif //ZOORK_PASSAGEDEFAULTUNLOCKCOMMAND_H
