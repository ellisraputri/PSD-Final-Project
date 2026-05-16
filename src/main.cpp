#include "Command/RoomDefaultEnterCommand.h"
#include "Passage/Passage.h"
#include "Character/Player.h"
#include "Room/Room.h"
#include "ZOOrkEngine.h"
#include<memory>


int main() {
    std::cout << "Welcome to Zoork! Have fun playing the game!\n\n";
    std::cout << "You open your eyes to see a familiar ceiling of your room. A spacious, glamorous, and elegantly designed room. A room that only fit for the high rank in the kingdom. A faint ticking of the clock made you realize what time it was, the time to meet your father, the king of the kingdom.\n\n";

    // std::shared_ptr<Room> start = std::make_shared<Room>("start-room",
    //     "You are standing in your bedroom. An elegant and glamorous room with several furnitures.\n");

    // std::shared_ptr<Room> south_of_house = std::make_shared<Room>("south-of-house",
    //                                 "You are facing the south side of a white house.  There is no door here, and all the windows are barred.\n");

    // std::shared_ptr<Room> behind_house = std::make_shared<Room>("behind-house",
    //                               "You are behind the white house. A path leads into the forest to the east. In one corner of the house there is a small window which is slightly ajar.\n");

    // Passage::createBasicPassage(start.get(), south_of_house.get(), "south", false, true);
    // Passage::createBasicPassage(south_of_house.get(), behind_house.get(), "east", false, true);

    ZOOrkEngine zoork; 
    zoork.run();

    return 0;
}
