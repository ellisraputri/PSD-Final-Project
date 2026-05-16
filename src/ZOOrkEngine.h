

#ifndef ZOORK_ZOORKENGINE_H
#define ZOORK_ZOORKENGINE_H

#include "Character/Player.h"
#include "Location.h"
#include "Passage/Passage.h"
#include "Information/Information.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class ZOOrkEngine {
public:
    ZOOrkEngine();

    void run();

private:
    bool gameOver = false;
    Player *player;
    Information info;

    void handleGoCommand(std::vector<std::string>);

    void handleLookCommand(std::vector<std::string>);

    void handleTakeCommand(std::vector<std::string>);

    void handleShowInventory();

    void handleDropCommand(std::vector<std::string>);

    void handleUseCommand(std::vector<std::string>);

    void handleAttackCommand(std::vector<std::string>);

    void handleQuitCommand(std::vector<std::string>);

    static std::vector<std::string> tokenizeString(const std::string&);

    static std::string makeLowercase(std::string);
};


#endif //ZOORK_ZOORKENGINE_H
