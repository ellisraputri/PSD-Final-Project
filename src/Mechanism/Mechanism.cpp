#include "Mechanism.h"

void Mechanism::setLocked(bool b) {
    locked = b;
}
bool Mechanism::isLocked() const {
    return locked;
}

void Mechanism::setResultPrint(std::string s) {
    resultPrint = s;
}
std::string Mechanism::getResultPrint() const {
    return resultPrint;
}