#ifndef PASSAGECONFIG_H
#define PASSAGECONFIG_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct PassageConfig {
    std::string name;
    std::string password;
    std::string fromRoom;
    std::string toRoom;
    std::string direction;
    bool locked;
    bool bidirectional;

    static PassageConfig fromJson(const json& data);
};

#endif