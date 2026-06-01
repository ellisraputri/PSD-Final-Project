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
    // std::cout <<"3a"<<std::endl;
    info->initItem();
    // std::cout << "4"<<std::endl;
    info->initCharacter();
    // std::cout << "5"<<std::endl;
    info->initRoomPopulation();
    // std::cout << "6"<<std::endl;
    info->initTrigger();
    // std::cout << "7"<<std::endl;
    info->initCheckpoints();
    // std::cout << "8" << std::endl;

    player = Player::instance();
    player->setCurrentRoom(info->getRoom("bedroom-bed").get());
    // std::cout << "8"<<std::endl;
    player->getCurrentRoom()->enter();
    // std::cout << "9"<<std::endl;
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "\n";
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
    std::vector<bool> isPrinted(arguments.size(), false);
    std::string str;

    for (int i=0; i<arguments.size(); i++){
        std::shared_ptr<Item> item = info->getItem(arguments[i]);
        if(item != nullptr){
            isPrinted[i] = true;
            std::cout << "\u001b[1;94m" << arguments[i] << "\u001b[0m : " << item->getDescription() << std::endl;
        }
    }
    if(arguments.size()>0 && std::find(isPrinted.begin(), isPrinted.end(), false) == isPrinted.end()) return;

    for (int i=0; i<arguments.size(); i++){
        if(isPrinted[i]) continue;

        std::shared_ptr<Mechanism> mechanism = info->getMechanism(arguments[i]);
        if(mechanism != nullptr){
            isPrinted[i] = true;
            
            if (mechanism->isLocked()){
                std::cout << "\u001b[1;94m" << arguments[i] << "\u001b[0m: " << mechanism->getDescription() << std::endl;
            } else {
                std::cout << "\u001b[1;94m" << arguments[i] << "\u001b[0m: " << mechanism->getResultPrint() << std::endl;
            }
        }
    }
    if(arguments.size()>0 && std::find(isPrinted.begin(), isPrinted.end(), false) == isPrinted.end()) return;

    for (int i=0; i<arguments.size(); i++){
        if(isPrinted[i]) continue;

        std::shared_ptr<Character> character = info->getCharacter(arguments[i]);
        if(character != nullptr){
            isPrinted[i] = true;
            std::cout << "\u001b[1;94m" << arguments[i] << "\u001b[0m: " << character->getDescription() << std::endl;
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
            std::cout << "make sure to type the item full name" << std::endl;
            continue;
        }

        if (!item->isTakeable()) {
            std::cout << s << " cannot be taken" << std::endl;
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

    std::cout << "Current inventory:" << std::endl;
    for (const std::shared_ptr<Item> item: inventory){
        std::cout << "* " << item->getName() << std::endl;
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
        std::cout << "make sure to type the character full name" << std::endl;
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
        std::cout << enemy->getName() << " has defeated" << std::endl;
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
        std::cout << "\n\u001b[1;94myou >>\u001b[0m ";

        std::getline(std::cin, dialogSelection);
        if (dialogSelection == "stop dialog") {
            break;
        }

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

    int input;
    std::cout << "Where do you want to teleport? Please just input the number.\n";

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

void ZOOrkEngine::handleHelpCommand(std::vector<std::string> arguments) {
    std::cout << "Command available: \n";
    std::cout << "\u001b[1;32m<go>\u001b[0m \u001b[1;91m<north | south | west | east | up | down>\u001b[0m =  move between rooms\n";
    std::cout << "\u001b[1;32m<look | inspect>\u001b[0m \u001b[1;91m<characterName | itemName | mechanismName>\u001b[0m = get details about specific thing\n";
    std::cout << "\u001b[1;32m<look | inspect>\u001b[0m = get details about current room \n";
    std::cout << "\u001b[1;32m<take | get>\u001b[0m \u001b[1;91m<itemName>\u001b[0m = take specific item into inventory\n";
    std::cout << "\u001b[1;32m<drop>\u001b[0m \u001b[1;91m<itemName>\u001b[0m = drop specific item from inventory\n" ;
    std::cout << "\u001b[1;32m<inventory>\u001b[0m = show current inventory\n";
    std::cout << "\u001b[1;32m<use>\u001b[0m \u001b[1;91m<itemName>\u001b[0m = use specific item\n";
    std::cout << "\u001b[1;32m<attack | hit>\u001b[0m \u001b[1;91m<characterName>\u001b[0m = hit specific character\n";
    std::cout << "\u001b[1;32m<talk | greet>\u001b[0m \u001b[1;91m<characterName>\u001b[0m = greet to specific character\n";
    std::cout << "\u001b[1;32m<dialog>\u001b[0m \u001b[1;91m<characterName>\u001b[0m = do dialog with specific character\n";
    std::cout << "\u001b[1;32m<teleport>\u001b[0m = teleport to specific room\n";
    std::cout << "\u001b[1;32m<unlock>\u001b[0m \u001b[1;91m<direction>\u001b[0m = unlock a locked passage based on direction\n";
    std::cout << "\u001b[1;32m<help>\u001b[0m = show all commands\n";
    std::cout << "\u001b[1;32m<quit | exit>\u001b[0m = stop the game, note: the game does not save your progress.\n";
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