#include "Information.h"

Information::Information(){}

std::string Information::getItemInfo(std::string name) {
    for(const Item& item: allItems){
        if (item.getName() == name){
            return item.getDescription();
        }
    }
    return "";
}

std::string Information::getMechanismInfo(std::string name) {
    for(const Mechanism& mechanism: allMechanisms){
        if (mechanism.getName() == name){
            return mechanism.getDescription();
        }
    }
    return "";
}