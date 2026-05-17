#include "Information.h"
#include "Command/PassageDefaultUnlockCommand.h"
#include "InformationData.h"

Information::Information(){}

std::shared_ptr<Item> Information::getItem(std::string name) {
    auto it = allItems.find(name);
    if (it != allItems.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Mechanism> Information::getMechanism(std::string name) {
    auto it = allMechanisms.find(name);
    if (it != allMechanisms.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Room> Information::getRoom(std::string name) {
    auto it = allRooms.find(name);
    if (it != allRooms.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Character> Information::getCharacter(std::string name) {
    auto it = allCharacters.find(name);
    if (it != allCharacters.end()) {
        return it->second;
    }
    return nullptr;
}

void Information::initRoom() {
    std::ifstream file("data/room.json");
    if (!file.is_open()) {
        std::cout << "cannot open room.json" << std::endl;
        throw std::runtime_error("Cannot open room.json");
    }

    json j;
    file >> j;

    for (const auto& data : j["rooms"]) {
        std::string name = data["name"];
        std::string desc = data["desc"];

        std::shared_ptr<Room> room = std::make_shared<Room>(name, desc);

        allRooms[name] = room;
    }
}

void Information::initPassage() {
    std::ifstream file("data/passage.json");
    if (!file.is_open()) {
        std::cout << "cannot open passage.json" << std::endl;
        throw std::runtime_error("Cannot open passage.json");
    }

    json j;
    file >> j;

    for(const auto& data: j["passages"]){
        std::string name = data["name"];
        std::string fromRoom = data["fromRoom"];
        std::string toRoom = data["toRoom"];
        std::string direction = data["direction"];
        bool locked = data["locked"];
        bool bidirectional = data["bidirectional"];
        
        auto passage = Passage::createBasicPassage(
            allRooms[fromRoom].get(),
            allRooms[toRoom].get(),
            direction,
            locked,
            bidirectional
        );
        
        allPassages[name + "_0"] = passage[0];
        allPassages[name + "_1"] = passage[1];
    }
}

void Information::initItem() {
    std::ifstream file("data/item.json");
    if (!file.is_open()) {
        std::cout << "cannot open item.json" << std::endl;
        throw std::runtime_error("Cannot open item.json");
    }

    json j;
    file >> j;

    for(const auto& data: j["items"]){
        std::string name = data["name"];
        std::string desc = data["desc"];
        std::string passage1 = data["passage1"];
        std::string passage2 = data["passage2"];

        std::shared_ptr<Item> item = std::make_shared<Item>(name, desc);
        if (passage1 != "") {
            auto unlockCommand = std::make_shared<PassageDefaultUnlockCommand>(
                allPassages[passage1].get(),
                allPassages[passage2].get(),
                item.get()
            );
            item->setUseCommand(unlockCommand);
        }
        allItems[name] = item;
    }
}

void Information::initCharacter(){
    std::ifstream file("data/character.json");
    if (!file.is_open()) {
        std::cout << "cannot open character.json" << std::endl;
        throw std::runtime_error("Cannot open character.json");
    }

    json j;
    file >> j;

    for(const auto& data: j["characters"]){
        std::string name = data["name"];
        std::string desc = data["desc"];
        int hp = data["hp"];
        int atk = data["atk"];
        int def = data["def"];
        bool combatMode = data["combatMode"];

        std::shared_ptr<Character> character = std::make_shared<Character>(
            name, desc, hp, atk, def, combatMode
        );
        allCharacters[name] = character;
    }
}

void Information::initRoomPopulation() {
    std::ifstream file("data/room.json");
    if (!file.is_open()) {
        std::cout << "cannot open room.json" << std::endl;
        throw std::runtime_error("Cannot open room.json");
    }

    json j;
    file >> j;

    for (const auto& data : j["rooms"]) {
        std::string name = data["name"];
        auto room = allRooms[name];

        for (const auto& itemName : data["items"]) {
            room->addItem(allItems[itemName]);
        }
        for (const auto& characterName : data["characters"]) {
            room->addCharacter(allCharacters[characterName]);
        }
    }
}

void Information::initTrigger() {
    std::ifstream file("data/trigger.json");
    if (!file.is_open()) {
        std::cout << "cannot open trigger.json" << std::endl;
        throw std::runtime_error("Cannot open trigger.json");
    }

    json j;
    file >> j;

    for (const auto& data: j["triggers"]){
        std::string triggerType = data["triggerType"];
        std::string target = data["target"];
        std::string flag = data["flag"];
        std::string result = data["result"];

        StoryTrigger trigger = StoryTrigger(stringToTriggerType(triggerType), target, flag, result);
        StoryManager::instance()->addTrigger(trigger);
    }
}