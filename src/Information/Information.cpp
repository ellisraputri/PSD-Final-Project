#include "Information.h"
#include "Item/BuffItem.h"
#include "Command/PassageDefaultUnlockCommand.h"
#include "Command/BuffCharacterItemCommand.h"

Information* Information::infoInstance = nullptr;

Information* Information::instance() {
    if (!infoInstance) {
        infoInstance = new Information();
    }
    return infoInstance;
}

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

void Information::setItem(const std::string& name, std::shared_ptr<Item> item) {
    allItems[name] = item;
}

void Information::setMechanism(const std::string& name, std::shared_ptr<Mechanism> mechanism) {
    allMechanisms[name] = mechanism;
}

void Information::setRoom(const std::string& name, std::shared_ptr<Room> room) {
    allRooms[name] = room;
}

void Information::setPassage(const std::string& name, std::shared_ptr<Passage> passage) {
    allPassages[name] = passage;
}

void Information::setCharacter(const std::string& name, std::shared_ptr<Character> character) {
    allCharacters[name] = character;
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
        
        auto passages = Passage::createBasicPassage(
            allRooms[fromRoom].get(),
            allRooms[toRoom].get(),
            direction,
            locked,
            bidirectional
        );
        
        std::string passageName = "passage_" + fromRoom + "_to_" + toRoom;
        allPassages[passageName] = passages[0];

        if (passages.size() > 1) {
            std::string passageName2 = "passage_" + toRoom + "_to_" + fromRoom;
            allPassages[passageName2] = passages[1];
        }
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
        std::string type = data["type"];

        std::shared_ptr<Item> item;

        if (type == "buff") {
            int hpBuff = data["hpBuff"];
            int atkBuff = data["atkBuff"];
            int defBuff = data["defBuff"];
            item = std::make_shared<BuffItem>(name, desc, hpBuff, atkBuff, defBuff);
            
            auto buffCommand = std::make_shared<BuffCharacterItemCommand>(
                item.get(), item.get()
            );
            item->setUseCommand(buffCommand);
        } 
        else {
            item = std::make_shared<Item>(name, desc);
        }

        if (type == "unlock"){
            std::string passage1 = data["passage1"];
            std::string passage2 = data["passage2"];
            
            if (passage1 != "") {
                auto unlockCommand = std::make_shared<PassageDefaultUnlockCommand>(
                    allPassages[passage1].get(),
                    allPassages[passage2].get(),
                    item.get()
                );
                item->setUseCommand(unlockCommand);
            }
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

        if (data.contains("dialogues")) {
            for (auto& [flag, text] : data["dialogues"].items()) {
                character->addDialogue(flag, text);
            }
        }

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
        std::string flagDone = data["flagDone"];
        std::string flagCondition = data["flagCondition"];
        bool lockPlayer = data["lockPlayer"];

        StoryTrigger trigger = StoryTrigger(stringToTriggerType(triggerType), target, flag, result, flagDone, flagCondition, lockPlayer);
        StoryManager::instance()->addTrigger(trigger);
    }
}

void Information::initCheckpoints(){
    checkpoints.push_back(allRooms["bedroom-bed"]);
    checkpoints.push_back(allRooms["corridor1"]);
    checkpoints.push_back(allRooms["library-entrace"]);
}

std::vector<std::string> Information::getCheckpointList(std::string current) {
    if (current == checkpoints[0]->getName()) {
        return std::vector<std::string>();
    }

    std::vector<std::string> returnList;
    int loopSize = 0;

    for (int i=0; i<checkpoints.size(); i++){
        if (current == checkpoints[i]->getName()){
            loopSize = i;
        }
    }

    loopSize = (loopSize % 2 ==1)? loopSize+1 : loopSize;
    for (int i=0; i<=loopSize; i++){
        returnList.push_back(checkpoints[i]->getName());
    }

    return returnList;
}