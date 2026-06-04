#include "TriggerConfig.h"

TriggerConfig TriggerConfig::fromJson(const json& data) {
    TriggerConfig config;

    config.triggerType = data["triggerType"];
    config.target = data["target"];
    config.flag = data["flag"];
    config.result = data["result"];
    config.flagDone = data["flagDone"];
    config.flagCondition = data["flagCondition"];
    config.lockPlayer = data["lockPlayer"];
    
    return config;
}