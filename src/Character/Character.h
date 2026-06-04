#ifndef ZOORK_CHARACTER_H
#define ZOORK_CHARACTER_H

#include "GameObject.h"
#include <vector>
#include <unordered_map>

class Character : public GameObject {
public:
    Character(const std::string &n, const std::string &d): GameObject(n,d){};

    void setCombatMode(bool);
    bool isCombatMode();

    std::string getDialogue(const std::string& flag) const;
    void printDialogues() const;
    void addDialogue(const std::string& flag, const std::string& dialogue);

protected:
    std::unordered_map<std::string, std::string> dialogues;
    bool combatMode = false;  
};

#endif //ZOORK_CHARACTER_H
