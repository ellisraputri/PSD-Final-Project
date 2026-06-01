#include "Command/RoomDefaultEnterCommand.h"
#include "Passage/Passage.h"
#include "Character/Player.h"
#include "Room/Room.h"
#include "ZOOrkEngine.h"
#include<memory>


int main() {
    std::cout << "\u001b[1;32mWelcome to Zoork!\u001b[0m \nIn this game, you will play in a perspective of a prince called \u001b[1;32mOtuwa\u001b[0m. \nHave fun playing the game! \n";
    std::cout << "\u001b[1;32mTips: type \"help\" to see what commands you can use\u001b[0m \n\n";
    std::cout << "You open your eyes to see a familiar ceiling of \u001b[91myour room\u001b[0m. \nA spacious, glamorous, and elegantly designed room. \n\n";

    ZOOrkEngine zoork; 
    zoork.run();

    return 0;
}
