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

void Character::addDialogue(const std::string& flag,const std::string& dialogue) {
    dialogues[flag] = dialogue;
}

std::string Character::getDialogue(const std::string& flag) const {
    auto it = dialogues.find(flag);
    if (it != dialogues.end()) {
        return it->second;
    }

    return "No dialog found for that key";
}

void Character::printDialogues() const {
    std::cout << "Please input the key to get the dialog you want. If you want to exit, just input 'stop dialog' \nKeys provided: \n";
    for (const auto& [key, val]: dialogues){
        std::cout << "* " << key << std::endl;
    }
}