#include "ZOOrkEngine.h"
#include <utility>
#include <algorithm>
#include <memory>
#include <random>

ZOOrkEngine::ZOOrkEngine() {
    info = Information::instance();
    info->initAll();

    player = Player::instance();
    player->setCurrentRoom(info->getRoom("bedroom-bed").get());
    player->getCurrentRoom()->enter();
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "\n> ";
        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = EngineUtils::tokenizeString(input);
        if (words.empty()) {
            continue;
        }
        std::string command = words[0];
        std::vector<std::string> arguments(words.begin() + 1, words.end());

        if (command == "go") {
            handleGoCommand(arguments);
        } else if ((command == "look") || (command == "inspect")) {
            handleLookCommand(arguments);
        } else if ((command == "take") || (command == "get")) {
            handleTakeCommand(arguments);
        } else if (command == "inventory") {
            handleShowInventory();
        } else if (command == "drop") {
            handleDropCommand(arguments);
        } else if (command == "use") {
            handleUseCommand(arguments);
        } else if (command == "attack" || command == "hit"){
            handleAttackCommand(arguments);
        } else if (command == "talk" || command == "greet") {
            handleTalkCommand(arguments);
        } else if (command == "dialog") {
            handleDialogCommand(arguments);
        } else if (command == "teleport") {
            handleTeleportCommand(arguments);
        } else if (command == "unlock") {
            handleUnlockPasswordCommand(arguments);
        } else if (command == "help") {
            handleHelpCommand(arguments);
        } else if (command == "quit" || command == "exit") {
            handleQuitCommand(arguments); 
        } else {
            std::cout << "I don't understand that command\n";
        }
    }
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
    if (player->isLocked()){
        std::cout << "You cannot move from your place right now.\n";
        return;
    }

    std::string direction;
    if (arguments[0] == "n" || arguments[0] == "north") {
        direction = "north";
    } else if (arguments[0] == "s" || arguments[0] == "south") {
        direction = "south";
    } else if (arguments[0] == "e" || arguments[0] == "east") {
        direction = "east";
    } else if (arguments[0] == "w" || arguments[0] == "west") {
        direction = "west";
    } else if (arguments[0] == "u" || arguments[0] == "up") {
        direction = "up";
    } else if (arguments[0] == "d" || arguments[0] == "down") {
        direction = "down";
    } else {
        direction = arguments[0];
    }

    Room* currentRoom = player->getCurrentRoom();
    auto passage = currentRoom->getPassage(direction);
    if (!passage->isLocked()){
        player->setCurrentRoom(passage->getTo());
    }
    passage->enter();
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    if (arguments.empty()){
        std::cout << player->getCurrentRoom()->getDescription()<< '\n';
    }

    std::vector<bool> isPrinted(arguments.size(), false);

    if (EngineUtils::processLookTargets(
        arguments,
        isPrinted,
        [this](const std::string& arg) {
            return isItemExist(arg) && player->getCurrentRoom()->isItemExist(arg);
        },
        [this](const std::string& arg) {
            std::cout<< "\u001b[1;94m"<< arg << "\u001b[0m: " << info->getItem(arg)->getDescription()<< '\n';
        }
    )) return;

    if (EngineUtils::processLookTargets(
        arguments,
        isPrinted,
        [this](const std::string& arg) {
            return isMechanismExist(arg);
        },
        [this](const std::string& arg) {
            auto mechanism = info->getMechanism(arg);
            std::cout << "\u001b[1;94m"<< arg<< "\u001b[0m: "<< (mechanism->isLocked()? mechanism->getDescription(): mechanism->getResultPrint()) << '\n';
        }
    )) return;

    if (EngineUtils::processLookTargets(
        arguments,
        isPrinted,
        [this](const std::string& arg) {
            return isCharacterExist(arg);
        },
        [this](const std::string& arg) {
            std::cout << "\u001b[1;94m" << arg << "\u001b[0m: " << info->getCharacter(arg)->getDescription() << '\n';
        }
    )) return;

    std::cout << player->getCurrentRoom()->getDescription()<< '\n';
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    for(const std::string& s: arguments){
        if(!isItemExist(s) || !player->getCurrentRoom()->isItemExist(s)) {
            std::cout << s << " is not a recognizable item, make sure to type the item full name" << std::endl;
            continue;
        };

        std::shared_ptr<Item> item = info->getItem(s);
        if (!item->isTakeable() || item->isTaken()) {
            std::cout << s << " cannot be taken\n";
            continue;
        }
        
        item->setIsTaken(true);
        currentRoom->removeItem(item);
        player->takeItem(item);
        std::cout << "\u001b[1;94m" << s << "\u001b[0m is taken into inventory\n";

        EventBus::instance()->emit({
            TriggerType::TAKE_ITEM,
            item->getName()
        });
    }
}

void ZOOrkEngine::handleShowInventory() {
    if (player->getInventory().empty()){
        std::cout << "Inventory is still empty" << std::endl;
        return;
    }

    std::cout << "Current inventory:" << std::endl;
    for (const std::shared_ptr<Item> item: player->getInventory()){
        std::cout << "* \u001b[1;32m" << item->getName() << "\u001b[0m" << std::endl;
    }
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    for(const std::string& s: arguments){
        if(!isItemExist(s) || !player->findItemFromInventory(s)) {
            std::cout << s << " is not a recognizable item, make sure to type the item full name" << std::endl;
            continue;
        };
        
        std::shared_ptr<Item> item = info->getItem(s);
        item->setIsTaken(false);
        player->dropItem(item);
        currentRoom->addItem(item);
        std::cout << "\u001b[1;32m" << s << "\u001b[0m is removed from inventory\n";
    }
}

void ZOOrkEngine::handleUseCommand(std::vector<std::string> arguments) {
    for(const std::string& s: arguments){
        if(!isItemExist(s) || !player->findItemFromInventory(s)) {
            std::cout << s << " is not a recognizable item, make sure to type the item full name" << std::endl;
            continue;
        };

        std::shared_ptr<Item> item = info->getItem(s);
        item->use();
        if(item->isUsed()) player->dropItem(item);
    }
}

void ZOOrkEngine::handleAttackCommand(std::vector<std::string> arguments) {
    if (arguments.size() != 1) {
        std::cout << "You can only attack one character at one time.\n";
        return;
    }
    if (!isCharacterExist(arguments[0])) {
        std::cout << arguments[0] << " is not a recognizable character, make sure to type the character full name" << std::endl;
        return;
    };

    auto character = info->getCharacter(arguments[0]);
    if (!character->isCombatMode()) {
        std::cout << character->getName() << " is not in a state to have a combat with you\n";
        return;
    }

    auto enemy = std::dynamic_pointer_cast<CombatCharacter>(character);
    std::string attackEnemyStr = enemy->getName() + " takes ";
    std::string hpEnemyStr = enemy->getName() + "'s health becomes ";

    if (performAttack(*player, *enemy, attackEnemyStr, hpEnemyStr)){
        std::cout << enemy->getName() << " has defeated\n";
        enemy->setCombatMode(false);

        EventBus::instance()->emit({
            TriggerType::WIN_COMBAT,
            enemy->getName()
        });
        return;
    }

    if (performAttack( *enemy, *player, "You take ", "Your health becomes ")) {
        std::cout << "You have died\n";
        gameOver = true;
    }
}

void ZOOrkEngine::handleTalkCommand(std::vector<std::string> arguments){
    Room* currentRoom = player->getCurrentRoom();

    for(const std::string& s: arguments){
        if(!isCharacterExist(s)) {
            std::cout << s << " is not a recognizable character, make sure to type the character full name" << std::endl;
            continue;
        }

        EventBus::instance()->emit({
            TriggerType::TALK_CHARACTER,
            s
        });
    }
}

void ZOOrkEngine::handleDialogCommand(std::vector<std::string> arguments){
    if (arguments.size() != 1){
        std::cout << "You can only dialog with one character at one time.\n";
        return;
    } 
    if (!isCharacterExist(arguments[0])) {
        std::cout << arguments[0] << " is not a recognizable character, make sure to type the character full name" << std::endl;
        return;
    }
    
    std::shared_ptr<Character> character = info->getCharacter(arguments[0]);
    std::string dialogSelection = "";
    character->printDialogues();

    while (dialogSelection != "stop dialog") {
        std::cout << "\n\u001b[1;94myou >>\u001b[0m ";

        std::getline(std::cin, dialogSelection);
        if (dialogSelection == "stop dialog") break;

        std::cout << "\u001b[1;91m" << character->getName() << " >>\u001b[0m " << character->getDialogue(dialogSelection) << "\n";
    }
}

void ZOOrkEngine::handleTeleportCommand(std::vector<std::string> arguments) {
    if (player->isLocked()){
        std::cout << "You cannot move from your place right now.\n";
        return;
    }
    
    auto checkpoints = info->getCheckpointList(player->getCheckpoint());
    if (checkpoints.empty()) {
        std::cout << "You have not unlocked any teleport checkpoint\n";
        return;
    }

    std::cout << "Where do you want to teleport?\n";
    for (int i = 0; i < checkpoints.size(); i++) {
        std::cout << "* " << checkpoints[i] << std::endl;
    }

    std::string selection = "";
    std::getline(std::cin, selection);
    if (find(checkpoints.begin(), checkpoints.end(), selection) == checkpoints.end()) {
        std::cout << "I don't know this place." << std::endl;
        return;
    }

    player->setCurrentRoom(info->getRoom(selection).get());
    std::cout << "You have teleported to \u001b[1;94m" << selection << "\u001b[0m" << std::endl;

    EventBus::instance()->emit({
        TriggerType::ENTER_ROOM,
        selection
    });
}

void ZOOrkEngine::handleUnlockPasswordCommand(std::vector<std::string> arguments) {
    if (arguments.size() != 1) {
        std::cout << "Usage: unlock <direction>\n";
        return;
    }

    std::string direction = arguments[0];
    auto room = player->getCurrentRoom();
    auto passage = room->getPassage(direction);
    
    if (dynamic_cast<NullPassage*>(passage.get()) != nullptr) {
        return;
    }

    auto passwordPassage = dynamic_cast<PasswordPassage*>(passage.get());
    if (passwordPassage == nullptr) {
        std::cout << "This passage does not use a password.\n";
        return;
    }

    std::string password;
    std::cout << "Enter password (lowercase only): ";
    std::cin >> password;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (passwordPassage->tryUnlock(password)) std::cout << "Passage unlocked.\n";
    else std::cout << "Wrong password.\n";
}

void ZOOrkEngine::handleHelpCommand(std::vector<std::string> arguments) {
    EngineUtils::printHelpCommand();
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::cin >> input;
    std::string quitStr = EngineUtils::makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
}

bool ZOOrkEngine::performAttack(CombatCharacter& attacker,CombatCharacter& defender,const std::string& attackText,const std::string& hpText) {
    int damage = EngineUtils::calculateDamage(attacker.getAtk(), defender.getDef());
    defender.takeDamage(damage);

    std::cout << attackText << damage << '\n';
    std::cout << hpText << defender.getHp() << '\n';
    return defender.isDead();
}

bool ZOOrkEngine::isItemExist(std::string name){
    std::shared_ptr<Item> item = info->getItem(name);
    if(item == nullptr){
        return false;
    }
    return true;
}

bool ZOOrkEngine::isMechanismExist(std::string name){
    std::shared_ptr<Mechanism> mechanism = info->getMechanism(name);
    if(mechanism == nullptr || !player->getCurrentRoom()->isMechanismExist(name)){
        return false;
    }
    return true;
}

bool ZOOrkEngine::isCharacterExist(std::string name){
    std::shared_ptr<Character> character = info->getCharacter(name);
    if(character == nullptr || !player->getCurrentRoom()->isCharacterExist(name)){
        return false;
    }
    return true;
}