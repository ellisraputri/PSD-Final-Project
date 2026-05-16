#include "Item.h"
#include "Command/PassageDefaultUnlockCommand.h"
#include <utility>

Item::Item(const std::string &n, const std::string &d) : GameObject(n, d),
    useCommand(std::make_shared<NullCommand>()) {
}

Item::Item(const std::string &n, const std::string &d, std::shared_ptr<Command> c) : GameObject(n, d),
    useCommand(std::move(c)) {
}

void Item::use() {
    auto cmd = std::dynamic_pointer_cast<ItemUseCommand>(useCommand);
    if (cmd) {
        cmd->setUsedItem(this);
    }

    useCommand->execute();
}

void Item::setUseCommand(std::shared_ptr<Command> c) {
    useCommand = c;
}

void Item::setIsTaken(bool b){
    taken = b;
}

bool Item::isTaken(){
    return taken;
}

void Item::setIsUsed(bool b){
    used = b;
}

bool Item::isUsed(){
    return used;
}