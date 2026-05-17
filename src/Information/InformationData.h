// GameData.h
#pragma once

#include <string>
#include <vector>

struct RoomData {
    std::string name;
    std::string desc;
};

struct PassageData {
    std::string name;
    std::string desc;

    std::string fromRoom;
    std::string toRoom;
    std::string direction;

    bool locked;
    bool bidirectional;
};

struct ItemData {
    std::string name;
    std::string desc;
    std::string passage1;
    std::string passage2;
};

struct CharacterData {
    std::string name;
    std::string desc;
    int hp;
    int atk;
    int def;
    bool combatMode;
};

struct RoomPopulationData {
    std::string roomName;
    std::vector<std::string> items;
    std::vector<std::string> characters;
};

struct TriggerData {
    std::string triggerType;
    std::string target;
    std::string flag; 
    std::string res;
};