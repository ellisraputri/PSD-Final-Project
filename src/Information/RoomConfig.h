#ifndef ROOMCONFIG_H
#define ROOMCONFIG_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct RoomConfig {
    std::string name;
    std::string description;
    std::vector<std::string> items;
    std::vector<std::string> mechanisms;
    std::vector<std::string> characters;

    static RoomConfig fromJson(const json& data);
};

#endif