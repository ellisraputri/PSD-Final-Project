#include "Information.h"

Information::Information(){}

Item* Information::getItem(std::string name) {
    for (Item& item : allItems) {
        if (item.getName() == name) {
            return &item;
        }
    }
    return nullptr;
}

Mechanism* Information::getMechanism(std::string name) {
    for(Mechanism& mechanism: allMechanisms){
        if (mechanism.getName() == name){
            return &mechanism;
        }
    }
    return nullptr;
}