

#include "Player.h"

Player *Player::playerInstance = nullptr;

void Player::setCurrentRoom(Room* room) {
    currentRoom = room;
}

Room* Player::getCurrentRoom() const {
    return currentRoom;
}

std::vector<Item*> Player::getInventory() const {
    return inventory;
}

void Player::takeItem(Item* item) {
    inventory.push_back(item);
}

void Player::dropItem(Item* item) {
    inventory.erase(
        std::remove(inventory.begin(), inventory.end(), item),
        inventory.end()
    );
}