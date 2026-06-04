#include "CharacterConfig.h"

CharacterConfig CharacterConfig::fromJson(const json& data) {
    CharacterConfig config;

    config.name = data["name"];
    config.description = data["desc"];
    config.combatMode = data["combatMode"];

    if (config.combatMode) {
        config.hp = data["hp"];
        config.atk = data["atk"];
        config.def = data["def"];
    }

    if (data.contains("dialogues")) {
        for (const auto& [flag, text] : data["dialogues"].items()) {
            config.dialogues[flag] = text;
        }
    }

    return config;
}