#ifndef ZOORK_CHARACTER_H
#define ZOORK_CHARACTER_H

#include "GameObject.h"
#include "Item/Item.h"
#include <vector>
#include <unordered_map>

class Character : public GameObject {
public:
    Character(const std::string &, const std::string &);
    Character(const std::string &n, const std::string &d, int h, int a, int df, bool c): 
        GameObject(n, d), hp(h), atk(a), def(df), combatMode(c){};

    void setHp(int);
    int getHp();
    void setAtk(int);
    int getAtk();
    void setDef(int);
    int getDef();
    void setCombatMode(bool);
    bool isCombatMode();

    void takeDamage(int);
    bool isDead();

    std::string getDialogue(const std::string& flag) const;

    void addDialogue(const std::string& flag, const std::string& dialogue);

protected:
    std::unordered_map<std::string, std::string> dialogues;
    int hp = 1;
    int atk = 0;
    int def = 0;
    bool combatMode = false;  
};

#endif //ZOORK_CHARACTER_H
