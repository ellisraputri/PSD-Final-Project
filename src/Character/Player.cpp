#include "Player.h"

Player *Player::playerInstance = nullptr;

void Player::setCurrentRoom(Room* room) {
    currentRoom = room;
}

Room* Player::getCurrentRoom() const {
    return currentRoom;
}

std::vector<std::shared_ptr<Item>> Player::getInventory() const {
    return inventory;
}

void Player::takeItem(std::shared_ptr<Item> item) {
    inventory.push_back(item);
}

void Player::dropItem(std::shared_ptr<Item> item) {
    inventory.erase(
        std::remove(inventory.begin(), inventory.end(), item),
        inventory.end()
    );
}

void Player::setLocked(bool b){
    locked = b;
}
bool Player::isLocked() const {
    return locked;
}

void Player::setCheckpoint(std::string s){
    checkpoint = s;
}
std::string Player::getCheckpoint() const {
    return checkpoint;
}