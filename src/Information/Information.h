#ifndef ZOORK_INFORMATION_H
#define ZOORK_INFORMATION_H

#include <vector>
#include "Item/Item.h"
#include "Mechanism/Mechanism.h"


class Information {
public:
    Information();

    std::string getItemInfo(std::string name);
    std::string getMechanismInfo(std::string name);

protected:
    std::vector<Item> allItems;
    std::vector<Mechanism> allMechanisms;
};

#endif //ZOORK_INFORMATION_H
