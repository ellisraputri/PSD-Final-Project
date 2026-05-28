#include "ZOOrkEngine.h"
#include <utility>
#include <algorithm>
#include <memory>
#include <random>

ZOOrkEngine::ZOOrkEngine() {
    info = Information::instance();
    // std::cout << "1"<<std::endl;
    info->initRoom();
    // std::cout << "2"<<std::endl;
    info->initPassage();
    // std::cout << "3"<<std::endl;
    info->initMechanism();
    info->initItem();
    // std::cout << "4"<<std::endl;
    info->initCharacter();
    // std::cout << "5"<<std::endl;
    info->initRoomPopulation();
    // std::cout << "6"<<std::endl;
    info->initTrigger();
    // std::cout << "7"<<std::endl;
    info->initCheckpoints();

    player = Player::instance();
    player->setCurrentRoom(info->getRoom("library-entrace").get());
    // std::cout << "8"<<std::endl;
    player->getCurrentRoom()->enter();
    // std::cout << "9"<<std::endl;
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
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
        }
        else if (command == "quit" || command == "exit") {
            handleQuitCommand(arguments); 
        }
        else {
            std::cout << "I don't understand that command\n";
        }

        std::cout << "\n";
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
    passage->enter();
    if (!passage->isLocked()){
        player->setCurrentRoom(passage->getTo());
    }
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    std::vector<bool> isPrinted(arguments.size(), false);
    std::string str;

    for (int i=0; i<arguments.size(); i++){
        std::shared_ptr<Item> item = info->getItem(arguments[i]);
        if(item != nullptr){
            isPrinted[i] = true;
            std::cout << arguments[i] << ": " << item->getDescription() << std::endl;
        }
    }
    if(arguments.size()>0 && std::find(isPrinted.begin(), isPrinted.end(), false) == isPrinted.end()) return;

    for (int i=0; i<arguments.size(); i++){
        if(isPrinted[i]) continue;

        std::shared_ptr<Mechanism> mechanism = info->getMechanism(arguments[i]);
        if(mechanism != nullptr){
            isPrinted[i] = true;
            
            if (mechanism->isLocked()){
                std::cout << arguments[i] << ": " << mechanism->getDescription() << std::endl;
            } else {
                std::cout << arguments[i] << ": " << mechanism->getResultPrint() << std::endl;
            }
        }
    }
    if(arguments.size()>0 && std::find(isPrinted.begin(), isPrinted.end(), false) == isPrinted.end()) return;

    for (int i=0; i<arguments.size(); i++){
        if(isPrinted[i]) continue;

        std::shared_ptr<Character> character = info->getCharacter(arguments[i]);
        if(character != nullptr){
            isPrinted[i] = true;
            std::cout << arguments[i] << ": " << character->getDescription() << std::endl;
        }
    }
    if(arguments.size()>0 && std::find(isPrinted.begin(), isPrinted.end(), false) == isPrinted.end()) return;

    Room* currentRoom = player->getCurrentRoom();
    std::cout << currentRoom->getDescription() << std::endl;
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    for(const std::string& s: arguments){
        std::shared_ptr<Item> item = info->getItem(s);
        if (item == nullptr || !currentRoom->isItemExist(item->getName())) {
            std::cout << s << " is not a recognizable item" << std::endl;
            continue;
        }

        if (item->isTaken()){
            std::cout << s << " is already taken before" << std::endl;
            continue;
        }
        
        item->setIsTaken(true);
        currentRoom->removeItem(item);
        player->takeItem(item);
        std::cout << s << " is taken into inventory" << std::endl;

        EventBus::instance()->emit({
            TriggerType::TAKE_ITEM,
            item->getName()
        });
    }
}

void ZOOrkEngine::handleShowInventory() {
    std::vector<std::shared_ptr<Item>> inventory = player->getInventory();
    if (inventory.empty()){
        std::cout << "Inventory is still empty" << std::endl;
        return;
    }

    std::cout << "Current Inventory:" << std::endl;
    for (const std::shared_ptr<Item> item: inventory){
        std::cout << item->getName() << std::endl;
    }
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    Room* currentRoom = player->getCurrentRoom();

    for(const std::string& s: arguments){
        std::shared_ptr<Item> item = info->getItem(s);
        if (item == nullptr) {
            std::cout << s << " is not a recognizable item" << std::endl;
            continue;
        }

        std::shared_ptr<Item> foundItem = nullptr;
        for (const auto& playerItem : player->getInventory()) {
            if (playerItem->getName() == s) {
                foundItem = item;
                break;
            }
        }
        if (foundItem == nullptr) {
            std::cout << s << " does not exist in the inventory" << std::endl;
            continue;
        }
        
        item->setIsTaken(false);
        player->dropItem(item);
        currentRoom->addItem(item);
        std::cout << s << " is removed from inventory" << std::endl;
    }
}

void ZOOrkEngine::handleUseCommand(std::vector<std::string> arguments) {
    for(const std::string& s: arguments){
        std::shared_ptr<Item> item = info->getItem(s);
        if (item == nullptr) {
            std::cout << s << " is not a recognizable item" << std::endl;
            continue;
        }

        std::shared_ptr<Item> foundItem = nullptr;
        for (const auto& playerItem : player->getInventory()) {
            if (playerItem->getName() == s) {
                foundItem = item;
                break;
            }
        }
        if (foundItem == nullptr) {
            std::cout << s << " does not exist in the inventory" << std::endl;
            continue;
        }
        
        item->use();
        if(item->isUsed()) {
            player->dropItem(item);
        }
    }
}

void ZOOrkEngine::handleAttackCommand(std::vector<std::string> arguments) {
    if (arguments.size() != 1){
        std::cout << "You can only attack one character at one time.\n";
        return;
    } 

    Room* currentRoom = player->getCurrentRoom();
    std::shared_ptr<Character> enemy = info->getCharacter(arguments[0]);

    if (enemy == nullptr || !currentRoom->isCharacterExist(enemy->getName())) {
        std::cout << enemy->getName() << " is not a recognizable enemy" << std::endl;
        return;
    }

    if (!enemy->isCombatMode()){
        std::cout << enemy->getName() << " is not in a state to have a combat with you" << std::endl;
        return;
    }
    
    int playerAtk = randomInt(0, std::max(player->getAtk(), 0));
    int playerDef = randomInt(0, std::max(player->getDef(), 0));
    int enemyAtk = randomInt(0, std::max(enemy->getAtk(), 0));
    int enemyDef = randomInt(0, std::max(enemy->getDef(), 0));

    int dmgFromPlayer = std::max((playerAtk - enemyDef), 0);
    enemy->takeDamage(dmgFromPlayer);
    std::cout << enemy->getName() << " takes " << dmgFromPlayer << " damage from you" << std::endl;
    std::cout << enemy->getName() << "'s health becomes " << enemy->getHp() << std::endl;

    if (enemy->isDead()){
        std::cout << enemy->getName() << " has died" << std::endl;
        enemy->setCombatMode(false);

        EventBus::instance()->emit({
            TriggerType::WIN_COMBAT,
            enemy->getName()
        });

        return;
    }

    int dmgFromEnemy = std::max((enemyAtk - playerDef), 0);
    player->takeDamage(dmgFromEnemy);
    std::cout << "You take " << dmgFromEnemy << " damage from " << enemy->getName() << std::endl;
    std::cout << "Your health becomes " << player->getHp() << std::endl;

    if (player->isDead()){
        std::cout << "You have died" << std::endl;
        gameOver = true;
        return;
    }
}

void ZOOrkEngine::handleTalkCommand(std::vector<std::string> arguments){
    Room* currentRoom = player->getCurrentRoom();

    for(const std::string& s: arguments){
        std::shared_ptr<Character> character = info->getCharacter(s);
        if (character == nullptr || !currentRoom->isCharacterExist(character->getName())) {
            std::cout << s << " is not a recognizable character" << std::endl;
            continue;
        }

        EventBus::instance()->emit({
            TriggerType::TALK_CHARACTER,
            character->getName()
        });
    }
}

void ZOOrkEngine::handleDialogCommand(std::vector<std::string> arguments){
    if (arguments.size() != 1){
        std::cout << "You can only dialog with one character at one time.\n";
        return;
    } 

    Room* currentRoom = player->getCurrentRoom();
    std::shared_ptr<Character> character = info->getCharacter(arguments[0]);
    if (character == nullptr || !currentRoom->isCharacterExist(character->getName())) {
        std::cout << arguments[0] << " is not a recognizable character" << std::endl;
        return;
    }

    std::string dialogSelection = "";
    character->printDialogues();

    while (dialogSelection != "stop dialog") {
        std::cout << "\nyou >> ";

        std::getline(std::cin, dialogSelection);
        if (dialogSelection == "stop dialog") {
            break;
        }

        std::cout << character->getName() << " >> " << character->getDialogue(dialogSelection) << "\n\n";
    }
}

void ZOOrkEngine::handleTeleportCommand(std::vector<std::string> arguments) {
    if (player->isLocked()){
        std::cout << "You cannot move from your place right now.\n";
        return;
    }

    int input;
    std::cout << "Where do you want to teleport? Please just input the number.\n";

    auto checkpoints = info->getCheckpointList(player->getCheckpoint());
    for (int i = 0; i < checkpoints.size(); i++) {
        std::cout << i << ". " << checkpoints[i] << std::endl;
    }

    if (!(std::cin >> input)) {
        std::cout << "Invalid input.\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (input < 0 || input >= checkpoints.size()) {
        std::cout << "I don't know this place." << std::endl;
        return;
    }

    std::shared_ptr<Room> room = info->getRoom(checkpoints[input]);
    player->setCurrentRoom(room.get());
    std::cout << "You have teleported to " << room->getName() << std::endl;

    EventBus::instance()->emit({
        TriggerType::ENTER_ROOM,
        room->getName()
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
    std::cout << "Enter password: ";
    std::cin >> password;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (passwordPassage->tryUnlock(password)) {
        std::cout << "Passage unlocked.\n";
    } else {
        std::cout << "Wrong password.\n";
    }
}

void ZOOrkEngine::handleQuitCommand(std::vector<std::string> arguments) {
    std::string input;
    std::cout << "Are you sure you want to QUIT?\n> ";
    std::cin >> input;
    std::string quitStr = makeLowercase(input);

    if (quitStr == "y" || quitStr == "yes") {
        gameOver = true;
    }
}

std::vector<std::string> ZOOrkEngine::tokenizeString(const std::string& input) {
    std::vector<std::string> tokens;

    if (input.empty()) {
        return tokens;
    }

    std::stringstream ss(input);

    // First word = command
    std::string command;
    ss >> command;

    tokens.push_back(makeLowercase(command));

    // Remaining text
    std::string rest;
    std::getline(ss, rest);

    // Replace commas with spaces
    std::replace(rest.begin(), rest.end(), ',', ' ');

    // Split remaining words
    std::stringstream argsStream(rest);
    std::string word;
    std::string currentItem;

    while (argsStream >> word) {
        word = makeLowercase(word);

        // Skip "and"
        if (word == "and") {
            if (!currentItem.empty()) {
                tokens.push_back(currentItem);
                currentItem.clear();
            }
            continue;
        }

        // Build multi-word item names
        if (!currentItem.empty()) {
            currentItem += " ";
        }

        currentItem += word;
    }

    // Push last item
    if (!currentItem.empty()) {
        tokens.push_back(currentItem);
    }

    return tokens;
}

std::string ZOOrkEngine::makeLowercase(std::string input) {
    std::string output = std::move(input);
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);

    return output;
}

int ZOOrkEngine::randomInt(int min, int max) {
    if (max < min) {
        std::swap(min, max);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}