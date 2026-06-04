#ifndef MECHANISMCONFIG_H
#define MECHANISMCONFIG_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct MechanismConfig {
    std::string name;
    std::string description;
    std::string print;

    static MechanismConfig fromJson(const json& data);
};

#endif