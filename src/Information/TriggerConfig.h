#ifndef TRIGGERCONFIG_H
#define TRIGGERCONFIG_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct TriggerConfig {
    std::string triggerType;
    std::string target;
    std::string flag;
    std::string result;
    std::string flagDone;
    std::string flagCondition;
    bool lockPlayer;

    static TriggerConfig fromJson(const json& data);
};

#endif