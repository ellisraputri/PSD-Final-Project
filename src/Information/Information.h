#ifndef ZOORK_INFORMATION_H
#define ZOORK_INFORMATION_H

#include <unordered_map>
#include "Item/Item.h"
#include "Mechanism/Mechanism.h"
#include "Room/Room.h"
#include "Passage/Passage.h"
#include "Story/TriggerType.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Information {
public:
    Information();

    std::shared_ptr<Item> getItem(std::string name);
    std::shared_ptr<Mechanism> getMechanism(std::string name);
    std::shared_ptr<Room> getRoom(std::string name);
    std::shared_ptr<Character> getCharacter(std::string name);

    void initRoom();
    void initPassage();
    void initItem();
    void initCharacter();
    void initRoomPopulation();
    void initTrigger();

protected:
    std::unordered_map<std::string, std::shared_ptr<Item>> allItems;
    std::unordered_map<std::string, std::shared_ptr<Mechanism>> allMechanisms;
    std::unordered_map<std::string, std::shared_ptr<Room>> allRooms;
    std::unordered_map<std::string, std::shared_ptr<Passage>> allPassages;
    std::unordered_map<std::string, std::shared_ptr<Character>> allCharacters;
};

#endif //ZOORK_INFORMATION_H
