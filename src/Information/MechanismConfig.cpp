#include "MechanismConfig.h"

MechanismConfig MechanismConfig::fromJson(const json& data) {
    MechanismConfig config;

    config.name = data["name"];
    config.description = data["desc"];
    config.print = data["print"];
    
    return config;
}