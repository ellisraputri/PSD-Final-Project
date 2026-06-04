#include "ItemConfig.h"

ItemConfig ItemConfig::fromJson(const json& data) {
    ItemConfig config;

    config.name = data["name"];
    config.description = data["desc"];
    config.type = data["type"];

    if (config.type == "buff") {
        config.hpBuff = data["hpBuff"];
        config.atkBuff = data["atkBuff"];
        config.defBuff = data["defBuff"];
    }

    if (config.type == "unlock-passage"){
        config.passage1 = data["passage1"];
        config.passage2 = data["passage2"];
    }

    if (config.type == "unlock-mechanism"){
        config.mechanism = data["mechanism"];
    }
    
    return config;
}