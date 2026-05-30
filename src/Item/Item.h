#ifndef ZOORK_ITEM_H
#define ZOORK_ITEM_H

#include "Command/Command.h"
#include "GameObject.h"
#include "Command/NullCommand.h"

class Item : public GameObject {
public:
    Item(const std::string &, const std::string &);

    Item(const std::string &, const std::string &, std::shared_ptr<Command>);

    virtual void use();

    void setUseCommand(std::shared_ptr<Command>);

    void setIsTaken(bool);
    bool isTaken();

    void setIsUsed(bool);
    bool isUsed();

    void setTakeable(bool);
    bool isTakeable();

protected:
    bool taken = false; 
    bool used = false;
    bool takeable = true;
    std::shared_ptr<Command> useCommand;
};

#endif //ZOORK_ITEM_H
