

#include "ZOOrkEngine.h"
#include "Information/Information.h"

#include <utility>
#include <algorithm>
#include <memory>

ZOOrkEngine::ZOOrkEngine(std::shared_ptr<Room> start) {
    player = Player::instance();
    player->setCurrentRoom(start.get());
    player->getCurrentRoom()->enter();

    info = Information();
}

void ZOOrkEngine::run() {
    while (!gameOver) {
        std::cout << "> ";

        std::string input;
        std::getline(std::cin, input);

        std::vector<std::string> words = tokenizeString(input);
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
        } else if (command == "quit") {
            handleQuitCommand(arguments);
        } else {
            std::cout << "I don't understand that command.\n";
        }
    }
}

void ZOOrkEngine::handleGoCommand(std::vector<std::string> arguments) {
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
    player->setCurrentRoom(passage->getTo());
    passage->enter();
}

void ZOOrkEngine::handleLookCommand(std::vector<std::string> arguments) {
    std::vector<bool> isPrinted(arguments.size(), false);
    std::string str;

    for (int i=0; i<arguments.size(); i++){
        Item* item = info.getItem(arguments[i]);
        if(item != nullptr){
            isPrinted[i] = true;
            std::cout << arguments[i] << ": " << item->getDescription() << std::endl;
        }
    }
    if(std::find(isPrinted.begin(), isPrinted.end(), false) == isPrinted.end()) return;

    for (int i=0; i<arguments.size(); i++){
        if(isPrinted[i]) continue;

        Mechanism* mechanism = info.getMechanism(arguments[i]);
        if(mechanism != nullptr){
            isPrinted[i] = true;
            std::cout << arguments[i] << ": " << mechanism->getDescription() << std::endl;
        }
    }
    if(std::find(isPrinted.begin(), isPrinted.end(), false) == isPrinted.end()) return;

    Room* currentRoom = player->getCurrentRoom();
    std::cout << currentRoom->getDescription() << std::endl;
}

void ZOOrkEngine::handleTakeCommand(std::vector<std::string> arguments) {
    for(const std::string& s: arguments){
        Item* item = info.getItem(s);
        if (item == nullptr) {
            std::cout << s << " is not a recognizable item" << std::endl;
            continue;
        }

        if (item->getIsTaken()){
            std::cout << s << " is already taken before" << std::endl;
            continue;
        }
        
        item->setIsTaken(true);
        player->takeItem(item);
        std::cout << s << " is taken into inventory" << std::endl;
    }
}

void ZOOrkEngine::handleShowInventory() {
    std::vector<Item*> inventory = player->getInventory();
    if (inventory.empty()){
        std::cout << "Inventory is still empty" << std::endl;
        return;
    }

    std::cout << "Current Inventory: \n" << std::endl;
    for (const Item* item: inventory){
        std::cout << item->getName() << std::endl;
    }
}

void ZOOrkEngine::handleDropCommand(std::vector<std::string> arguments) {
    for(const std::string& s: arguments){
        Item* item = info.getItem(s);
        if (item == nullptr) {
            std::cout << s << " is not a recognizable item" << std::endl;
            continue;
        }

        if (!item->getIsTaken()){
            std::cout << s << " does not exist in inventory" << std::endl;
            continue;
        }
        
        item->setIsTaken(false);
        player->dropItem(item);
        std::cout << s << " is removed from inventory" << std::endl;
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
