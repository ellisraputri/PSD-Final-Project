#include "PassageConfig.h"

PassageConfig PassageConfig::fromJson(const json& data) {
    PassageConfig config;

    config.name = data["name"];
    config.password = data["password"];
    config.fromRoom = data["fromRoom"];
    config.toRoom = data["toRoom"];
    config.direction = data["direction"];
    config.locked = data["locked"];
    config.bidirectional = data["bidirectional"];

    return config;
}