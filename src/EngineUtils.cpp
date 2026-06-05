#include "EngineUtils.h"

std::vector<std::string> EngineUtils::tokenizeString(const std::string& input) {
    std::vector<std::string> tokens;
    if (input.empty()) {
        return tokens;
    }

    std::stringstream ss(input);
    std::string command;
    ss >> command;

    tokens.push_back(makeLowercase(command));
    std::string rest;
    std::getline(ss, rest);

    std::replace(rest.begin(), rest.end(), ',', ' ');

    std::stringstream argsStream(rest);
    std::string word;
    std::string currentItem;

    while (argsStream >> word) {
        word = makeLowercase(word);

        if (word == "and") {
            if (!currentItem.empty()) {
                tokens.push_back(currentItem);
                currentItem.clear();
            }
            continue;
        }

        if (!currentItem.empty()) {
            currentItem += " ";
        }

        currentItem += word;
    }

    if (!currentItem.empty()) {
        tokens.push_back(currentItem);
    }

    return tokens;
}

std::string EngineUtils::makeLowercase(std::string input) {
    std::string output = std::move(input);
    std::transform(output.begin(), output.end(), output.begin(), ::tolower);

    return output;
}

int EngineUtils::randomInt(int min, int max) {
    if (max < min) {
        std::swap(min, max);
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

bool EngineUtils::processLookTargets(const std::vector<std::string>& arguments, std::vector<bool>& isPrinted, std::function<bool(const std::string&)> exists, std::function<void(const std::string&)> print){
    bool allHandled = true;

    for (int i = 0; i < arguments.size(); i++) {
        if (isPrinted[i]) continue;

        if (exists(arguments[i])) {
            isPrinted[i] = true;
            print(arguments[i]);
        }

        if (!isPrinted[i]) allHandled = false;
    }

    return allHandled;
}

int EngineUtils::calculateDamage(int atk, int def) {
    return std::max(randomInt(0, std::max(atk, 0)) - randomInt(0, std::max(def, 0)), 0);
}

void EngineUtils::printHelpCommand(){
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