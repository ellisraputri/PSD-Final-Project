#ifndef ZOORK_PLAYER_H
#define ZOORK_PLAYER_H

#include "CombatCharacter.h"
#include "Location.h"
#include "Room/NullRoom.h"
#include <vector>

class Player : public CombatCharacter {
public:
    static Player *instance() {
        // Note: lazy instantiation of the singleton Player object
        if (!playerInstance) {
            playerInstance = new Player();
        }
        return Player::playerInstance;
    }

    void setCurrentRoom(Room*);
    Room* getCurrentRoom() const;

    std::vector<std::shared_ptr<Item>> getInventory() const;
    void takeItem(std::shared_ptr<Item>);
    void dropItem(std::shared_ptr<Item>);

    void setLocked(bool);
    bool isLocked() const;

    void setCheckpoint(std::string);
    std::string getCheckpoint() const;

    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

private:
    static Player *playerInstance;
    Room* currentRoom;
    std::vector<std::shared_ptr<Item>> inventory;
    bool locked = false;
    std::string checkpoint = "bedroom-bed";

    Player() : CombatCharacter("You", "You are a person, alike in dignity to any other, but uniquely you."),
               currentRoom(new NullRoom()) {}
};

#endif //ZOORK_PLAYER_H
