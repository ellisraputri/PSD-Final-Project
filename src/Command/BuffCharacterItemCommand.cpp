#include "BuffCharacterItemCommand.h"
#include <iostream>
#include "Item/BuffItem.h"
#include "Character/Player.h"

void BuffCharacterItemCommand::execute() {
    BuffItem* item = static_cast<BuffItem*>(gameObject);

    if (usedItem == requiredItem) {
        Player* player = Player::instance();
        player->setAtk(player->getAtk() + item->getAtkBuff());
        player->setDef(player->getDef() + item->getDefBuff());
        player->setHp(player->getHp() + item->getHpBuff());

        usedItem->setIsUsed(true);
        std::cout << "You used the item \u001b[1;94m"+ item->getName() + "\u001b[0m and get an increase in your combat stats\n";
        std::cout << "HP stats: "<< player->getHp() << "\n";
        std::cout << "ATK stats: "<< player->getAtk() << "\n";
        std::cout << "DEF stats: "<< player->getDef() << "\n";
        return;
    }

    std::cout << "That item cannot be used right now.\n";
}