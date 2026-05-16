#ifndef ZOORK_INFORMATION_H
#define ZOORK_INFORMATION_H

#include <vector>
#include "Item/Item.h"
#include "Mechanism/Mechanism.h"
#include "Room/Room.h"
#include "Passage/Passage.h"

class Information {
public:
    Information();

    std::shared_ptr<Item> getItem(std::string name);
    std::shared_ptr<Mechanism> getMechanism(std::string name);
    std::shared_ptr<Room> getRoom(std::string name);

    void initItem();

protected:
    std::vector<std::shared_ptr<Item>> allItems;
    std::vector<std::shared_ptr<Mechanism>> allMechanisms;
    std::vector<std::shared_ptr<Room>> allRooms;
    std::vector<std::shared_ptr<Passage>> allPassages;
};

#endif //ZOORK_INFORMATION_H
