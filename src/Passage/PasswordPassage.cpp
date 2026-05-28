#include "PasswordPassage.h"
#include "Information/Information.h"

PasswordPassage::PasswordPassage(
    const std::string& name,
    const std::string& description,
    Room* from,
    Room* to,
    const std::string& password
)
    : Passage(name, description, from, to),
      password(password)
{
    locked = true;
}

void PasswordPassage::setPassword(std::string p) {
    password = p;
}

bool PasswordPassage::tryUnlock(const std::string& inputPassword) {
    if (inputPassword == password) {
        locked = false;

        std::string otherName = "passage_" + toRoom->getName() + "_to_" + fromRoom->getName();
        std::shared_ptr<Passage> otherPassage = Information::instance()->getPassage(otherName);
        otherPassage->setLocked(false);

        return true;
    }

    return false;
}