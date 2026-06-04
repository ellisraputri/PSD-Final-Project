#ifndef ITEMCONFIG_H
#define ITEMCONFIG_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct ItemConfig {
    std::string name;
    std::string description;
    std::string type;
    
    int hpBuff;
    int atkBuff;
    int defBuff;
    
    std::string passage1;
    std::string passage2;
    std::string mechanism;

    static ItemConfig fromJson(const json& data);
};

#endif