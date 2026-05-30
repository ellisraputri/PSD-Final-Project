#include "Command/RoomDefaultEnterCommand.h"
#include "Passage/Passage.h"
#include "Character/Player.h"
#include "Room/Room.h"
#include "ZOOrkEngine.h"
#include<memory>


int main() {
    std::cout << "Welcome to Zoork! \nIn this game, you will play in a perspective of a prince called Otuwa. \nHave fun playing the game! \n\n";
    std::cout << "You open your eyes to see a familiar ceiling of your room. \nA spacious, glamorous, and elegantly designed room. \nA room that only fit for the high rank in the kingdom. \n\n";

    ZOOrkEngine zoork; 
    zoork.run();

    return 0;
}
