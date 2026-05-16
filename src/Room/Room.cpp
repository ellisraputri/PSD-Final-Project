#include "Passage/NullPassage.h"
#include "Room.h"

#include <utility>

Room::Room(const std::string &n, const std::string &d) : Location(n, d) {
    enterCommand = std::make_shared<RoomDefaultEnterCommand>(this);
}

Room::Room(const std::string &n, const std::string &d, std::shared_ptr<Command> c) : Location(n, d, std::move(c)) {}

std::string Room::getDescription() const {
    std::string desc = Location::getDescription();
    for (const auto& item: items){
        desc += "\n" + item->getDescription();
    }
    for (const auto& character: characters){
        desc += "\n" + character->getDescription();
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

bool Room::isItemExist(const std::shared_ptr<Item>& item) {
    return std::find(items.begin(), items.end(), item) != items.end();
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

bool Room::isCharacterExist(const std::shared_ptr<Character>& ch) {
    return std::find(characters.begin(), characters.end(), ch) != characters.end();
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