#ifndef ZOORK_COMBATCHARACTER_H
#define ZOORK_COMBATCHARACTER_H

#include "Character.h"
#include "Item/Item.h"
#include <vector>
#include <unordered_map>

class CombatCharacter : public Character {
public:
    CombatCharacter(const std::string &n, const std::string &d): Character(n, d){};
    CombatCharacter(const std::string &n, const std::string &d, int h, int a, int df);

    void setHp(int);
    int getHp();
    void setAtk(int);
    int getAtk();
    void setDef(int);
    int getDef();

    void takeDamage(int);
    bool isDead();

protected:
    int hp = 100;
    int atk = 100;
    int def = 100;
};

#endif //ZOORK_COMBATCHARACTER_H
