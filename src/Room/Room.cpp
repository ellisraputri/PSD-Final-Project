#include "Passage/NullPassage.h"
#include "Room.h"

#include <utility>

Room::Room(const std::string &n, const std::string &d) : Location(n, d) {
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
}

Room::Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c) : Location(n, d, std::move(c)) {}

std::string Room::getDescription() const {
    std::string desc = GameObject::getDescription();
    int itemSize = items.size();
    int charSize = characters.size();
    int mechanismSize = mechanisms.size();
    
    if (itemSize != 0) desc += "\nItems in the room: ";
    for (int i=0; i<itemSize; i++){
        desc += "\u001b[1;94m" + items[i]->getName() + "\u001b[0m" + ((i != itemSize-1)? ", " : "");
    }

    if (charSize != 0) desc += "\nCharacters in the room: ";
    for (int i=0; i<charSize; i++){
        desc += "\u001b[1;94m" + characters[i]->getName() + "\u001b[0m" + ((i != charSize-1)? ", " : "");
    }

    if (mechanismSize != 0) desc += "\nMechanisms in the room: ";
    for (int i=0; i<mechanismSize; i++){
        desc += "\u001b[1;94m" + mechanisms[i]->getName() + "\u001b[0m" + ((i != mechanismSize-1)? ", " : "");
    }

    return desc;
}

void Room::addItem(std::shared_ptr<Item> item){
    items.push_back(item);
}

void Room::removeItem(std::shared_ptr<Item> item){
    items.erase(
        std::remove(items.begin(), items.end(), item),
        items.end()
    );
}

bool Room::isItemExist(const std::string& s) {
    for (auto& i: items){
        if (i->getName() == s) return true;
    }
    return false;
}

void Room::addCharacter(std::shared_ptr<Character> ch){
    characters.push_back(ch);
}

void Room::removeCharacter(std::shared_ptr<Character> ch){
    characters.erase(
        std::remove(characters.begin(), characters.end(), ch),
        characters.end()
    );
}

bool Room::isCharacterExist(const std::string& s) {
    for (auto& c: characters){
        if (c->getName() == s) return true;
    }
    return false;
}

void Room::addPassage(const std::string &direction, std::shared_ptr<Passage> p) {
    passageMap[direction] = std::move(p);
}

void Room::removePassage(const std::string &direction) {
    if (passageMap.find(direction) != passageMap.end()) {
        passageMap.erase(direction);
    }
}

std::shared_ptr<Passage> Room::getPassage(const std::string &direction) {
    if (passageMap.find(direction) != passageMap.end()) {
        return passageMap[direction];
    } else {
        std::cout << "It is impossible to go " << direction << "!\n";
        return std::make_shared<NullPassage>(this);
    }
}

bool Room::isPassageExist(const std::string& s) {
    for (auto& [key, val]: passageMap){
        if (val->getName() == s) return true;
    }
    return false;
}

void Room::addMechanism(std::shared_ptr<Mechanism> mechanism){
    mechanisms.push_back(mechanism);
}

void Room::removeMechanism(std::shared_ptr<Mechanism> mechanism){
    mechanisms.erase(
        std::remove(mechanisms.begin(), mechanisms.end(), mechanism),
        mechanisms.end()
    );
}

bool Room::isMechanismExist(const std::string& s) {
    for (auto& m: mechanisms){
        if (m->getName() == s) return true;
    }
    return false;
}

void Room::enter() {
    std::cout << "\u001b[1;94m" << getName() << "\u001b[0m" << std::endl;
    std::cout << getDescription() << std::endl;

    EventBus::instance()->emit({
        TriggerType::ENTER_ROOM,
        getName()
    });
}
