#include "Information.h"
#include "Command/PassageDefaultUnlockCommand.h"

struct RoomData {
    std::string name;
    std::string desc;
};
struct PassageData {
    std::string name;
    std::string desc;
    std::shared_ptr<Room> room1;
    std::shared_ptr<Room> room2;
};
struct ItemData {
    std::string name;
    std::string desc;
    std::shared_ptr<Passage> passage1;
    std::shared_ptr<Passage> passage2; //bidirectional
};
struct CharacterData {
    std::string name;
    std::string desc;
    int hp;
    int atk;
    int def;
    bool combatMode;
};

Information::Information(){}

std::shared_ptr<Item> Information::getItem(std::string name) {
    for (const auto& item : allItems) {
        if (item->getName() == name) {
            return item;
        }
    }
    return nullptr;
}

std::shared_ptr<Mechanism> Information::getMechanism(std::string name) {
    for(const auto& mechanism: allMechanisms){
        if (mechanism->getName() == name){
            return mechanism;
        }
    }
    return nullptr;
}

std::shared_ptr<Room> Information::getRoom(std::string name) {
    for (const auto& room : allRooms) {
        if (room->getName() == name) {
            return room;
        }
    }
    return nullptr;
}

std::shared_ptr<Character> Information::getCharacter(std::string name) {
    for (const auto& ch : allCharacters) {
        if (ch->getName() == name) {
            return ch;
        }
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

    for(int i=0; i<rooms.size(); i++){
        std::shared_ptr<Room> room = std::make_shared<Room>(rooms[i].name, rooms[i].desc);
        allRooms.push_back(room);
    }

    std::vector<PassageData> passages = {
        {
            "passage1",
            "description1",
            allRooms[0],
            allRooms[1]
        }
    };

    for(int i=0; i<passages.size(); i++){
        std::vector<std::shared_ptr<Passage>> passage = Passage::createBasicPassage(
            passages[i].room1.get(), 
            passages[i].room2.get(),
            "south",
            true,   //locked 
            true   //bidirectional
        );
        allPassages.push_back(passage[0]);
        allPassages.push_back(passage[1]);
    }

    std::vector<ItemData> items = {
        {
            "item1",
            "description1 item1",
            nullptr
        },
        {
            "item2",
            "desc2 item2",
            allPassages[0],
            allPassages[1], //bidirectional
        }  
    };

    for(int i=0; i<items.size(); i++){
        std::shared_ptr<Item> item = std::make_shared<Item>(items[i].name, items[i].desc);
        if (items[i].passage1 != nullptr) {
            auto unlockCommand = std::make_shared<PassageDefaultUnlockCommand>(
                items[i].passage1.get(),
                items[i].passage2.get(),
                item.get()
            );
            item->setUseCommand(unlockCommand);
        }
        allItems.push_back(item);
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

    for(int i=0; i<characters.size(); i++){
        std::shared_ptr<Character> ch = std::make_shared<Character>(
            characters[i].name, 
            characters[i].desc,
            characters[i].hp,
            characters[i].atk,
            characters[i].def,
            characters[i].combatMode
        );
        allCharacters.push_back(ch);
    }

    // initialize items and characters in the room
    allRooms[0]->addItem(allItems[0]);
    allRooms[0]->addItem(allItems[1]);
    allRooms[1]->addCharacter(allCharacters[0]);
}