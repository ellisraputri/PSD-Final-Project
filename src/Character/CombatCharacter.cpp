#include "CombatCharacter.h"

CombatCharacter::CombatCharacter(const std::string &n, const std::string &d, int h, int a, int df): 
    Character(n, d), hp(h), atk(a), def(df){
        setCombatMode(true);
    }

void CombatCharacter::setHp(int h){
    hp = h;
}
int CombatCharacter::getHp() {
    return hp;
}
void CombatCharacter::setAtk(int a){
    atk = a;
}
int CombatCharacter::getAtk(){
    return atk;
}
void CombatCharacter::setDef(int d){
    def = d;
}
int CombatCharacter::getDef() {
    return def;
}

void CombatCharacter::takeDamage(int dmg){
    hp -= dmg;
    if (hp < 0){
        hp = 0;
    }
}

bool CombatCharacter::isDead() {
    return hp <= 0;
}