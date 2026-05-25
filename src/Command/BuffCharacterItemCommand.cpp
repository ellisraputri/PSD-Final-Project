#include "BuffCharacterItemCommand.h"
#include <iostream>
#include "Item/BuffItem.h"
#include "Character/Player.h"

void BuffCharacterItemCommand::execute() {
    BuffItem* item = static_cast<BuffItem*>(gameObject);

    if (usedItem == requiredItem) {
        Player* player = Player::instance();
        int playerAtk = player->getAtk();
        int playerDef = player->getDef();
        int playerHp = player->getHp();

        player->setAtk(playerAtk + item->getAtkBuff());
        player->setDef(playerDef + item->getDefBuff());
        player->setHp(playerHp + item->getHpBuff());

        usedItem->setIsUsed(true);
        std::cout << "You used the item "+ item->getName() + " and get an increase in your combat stats\n";
        std::cout << "HP stats: "<< player->getHp() << "\n";
        std::cout << "ATK stats: "<< player->getAtk() << "\n";
        std::cout << "DEF stats: "<< player->getDef() << "\n";
        return;
    }

    std::cout << "That item cannot be used right now.\n";
}