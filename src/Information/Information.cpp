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

void Information::initItem() {
    std::vector<RoomData> rooms = {
        {
            "start-room",
            "description1 start-room"
        },
        {
            "second-room",
            "desc2 - second room"
        }
    };

    for(const auto& data: rooms){
        std::shared_ptr<Room> room = std::make_shared<Room>(data.name, data.desc);
        allRooms[data.name] = room;
    }

    std::vector<PassageData> passages = {
        {
            "passage1",
            "description1",
            "start-room",
            "second-room",
            "south",
            true,
            true,
        }
    };

    for(const auto& data: passages){
        auto passage = Passage::createBasicPassage(
            allRooms[data.fromRoom].get(),
            allRooms[data.toRoom].get(),
            data.direction,
            data.locked,
            data.bidirectional
        );
        allPassages[data.name + "_0"] = passage[0];
        allPassages[data.name + "_1"] = passage[1];
    }

    std::vector<ItemData> items = {
        {
            "item1",
            "description1 item1",
            "",
            ""
        },
        {
            "item2",
            "desc2 item2",
            "passage1_0",
            "passage1_1",
        }  
    };

    for(const auto& data: items){
        std::shared_ptr<Item> item = std::make_shared<Item>(data.name, data.desc);
        if (data.passage1 != "") {
            auto unlockCommand = std::make_shared<PassageDefaultUnlockCommand>(
                allPassages[data.passage1].get(),
                allPassages[data.passage2].get(),
                item.get()
            );
            item->setUseCommand(unlockCommand);
        }
        allItems[data.name] = item;
    }

    std::vector<CharacterData> characters = {
        {
            "char1",
            "char1 description",
            1, //hp
            1, //atk
            0, //def
            true //combatMode
        }
    };

    for(const auto& data: characters){
        std::shared_ptr<Character> character = std::make_shared<Character>(
            data.name, 
            data.desc,
            data.hp,
            data.atk,
            data.def,
            data.combatMode
        );
        allCharacters[data.name] = character;
    }

    std::vector<RoomPopulationData> roomPopulations = {
        {
            "start-room",
            {"item1", "item2"},
            {"char1"}
        }
    };

    for (const auto& data : roomPopulations) {
        auto room = allRooms[data.roomName];
        for (const auto& itemName : data.items) {
            room->addItem(allItems[itemName]);
        }
        for (const auto& characterName : data.characters) {
            room->addCharacter(allCharacters[characterName]);
        }
    }
}