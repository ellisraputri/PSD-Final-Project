#include "RoomConfig.h"

RoomConfig RoomConfig::fromJson(const json& data) {
    RoomConfig config;

    config.name = data["name"];
    config.description = data["desc"];

    for (const auto& itemName : data["items"]) {
        config.items.push_back(itemName);
    }
    for (const auto& characterName : data["characters"]) {
        config.characters.push_back(characterName);
    }
    for (const auto& mechanismName : data["mechanisms"]) {
        config.mechanisms.push_back(mechanismName);
    }
    
    return config;
}