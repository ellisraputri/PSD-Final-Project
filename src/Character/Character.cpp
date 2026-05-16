#include "Character.h"


Character::Character(const std::string &n, const std::string &d) : GameObject(n, d) {}

void Character::setHp(int h){
    hp = h;
}
int Character::getHp() {
    return hp;
}
void Character::setAtk(int a){
    atk = a;
}
int Character::getAtk(){
    return atk;
}
void Character::setDef(int d){
    def = d;
}
int Character::getDef() {
    return def;
}
void Character::setCombatMode(bool b){
    combatMode = b;
}
bool Character::isCombatMode(){
    return combatMode;
}

void Character::takeDamage(int dmg){
    hp -= dmg;
    if (hp < 0){
        hp = 0;
    }
}

bool Character::isDead() {
    return hp <= 0;
}