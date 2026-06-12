#ifndef ZOORK_MECHANISM_H
#define ZOORK_MECHANISM_H

#include "GameObject.h"

class Mechanism : public GameObject {
public:
    Mechanism(const std::string name, const std::string desc, std::string r)
    : GameObject(name, desc), resultPrint(r){}

    void setLocked(bool);
    bool isLocked() const;

    void setResultPrint(std::string);
    std::string getResultPrint() const;

private:
   bool locked = true;
   std::string resultPrint;
};

#endif //ZOORK_MECHANISM_H
