#ifndef CHARACTERCONFIG_H
#define CHARACTERCONFIG_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct CharacterConfig {
    std::string name;
    std::string description;
    bool combatMode;
    int hp;
    int atk;
    int def;
    std::map<std::string, std::string> dialogues;

    static CharacterConfig fromJson(const json& data);
};

#endif