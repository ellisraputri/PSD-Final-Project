#ifndef ZOORK_INFORMATION_H
#define ZOORK_INFORMATION_H

#include <unordered_map>
#include <vector>
#include "PassageConfig.h"
#include "RoomConfig.h"
#include "MechanismConfig.h"
#include "ItemConfig.h"
#include "CharacterConfig.h"
#include "TriggerConfig.h"
#include "Item/Item.h"
#include "Item/BuffItem.h"
#include "Mechanism/Mechanism.h"
#include "Room/Room.h"
#include "Passage/Passage.h"
#include "Passage/PasswordPassage.h"
#include "Command/PassageDefaultUnlockCommand.h"
#include "Command/BuffCharacterItemCommand.h"
#include "Command/MechanismUnlockCommand.h"
#include "Story/TriggerType.h"
#include "Story/StoryManager.h"
#include "Story/StoryTrigger.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Information {
public:
    static Information* instance();

    std::shared_ptr<Item> getItem(std::string name);
    std::shared_ptr<Mechanism> getMechanism(std::string name);
    std::shared_ptr<Room> getRoom(std::string name);
    std::shared_ptr<Character> getCharacter(std::string name);
    std::shared_ptr<Passage> getPassage(std::string name);

    void setItem(const std::string& name, std::shared_ptr<Item> item);
    void setMechanism(const std::string& name, std::shared_ptr<Mechanism> mechanism);
    void setRoom(const std::string& name, std::shared_ptr<Room> room);
    void setPassage(const std::string& name, std::shared_ptr<Passage> passage);
    void setCharacter(const std::string& name, std::shared_ptr<Character> character);

    void initAll();
    void initRoom();
    void initPassage();
    void initItem();
    void initMechanism();
    void initCharacter();
    void initRoomPopulation();
    void initTrigger();
    void initCheckpoints();

    std::vector<std::string> getCheckpointList(std::string);

private:
    static Information* infoInstance;

    std::unordered_map<std::string, std::shared_ptr<Item>> allItems;
    std::unordered_map<std::string, std::shared_ptr<Mechanism>> allMechanisms;
    std::unordered_map<std::string, std::shared_ptr<Room>> allRooms;
    std::unordered_map<std::string, std::shared_ptr<Passage>> allPassages;
    std::unordered_map<std::string, std::shared_ptr<Character>> allCharacters;
    std::vector<std::shared_ptr<Room>> checkpoints;

    json readJson(std::string);
    std::vector<std::shared_ptr<Passage>> createPassage(PassageConfig&);
    std::shared_ptr<BuffItem> createBuffItem(ItemConfig&);
    std::shared_ptr<Item> createUnlockPassageItem(ItemConfig&);
    std::shared_ptr<Item> createUnlockMechanismItem(ItemConfig&);
    std::shared_ptr<Item> createDecorativeItem(ItemConfig&);
};

#endif