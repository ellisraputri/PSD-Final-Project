#ifndef ZOORK_PASSAGE_H
#define ZOORK_PASSAGE_H

#include "Room/NullRoom.h"
#include "Room/Room.h"
#include <iostream>

class Passage : public Location {
public:
    static std::shared_ptr<Passage> createBasicPassage(Room*, Room*, const std::string &, bool, bool);
    Passage(const std::string &, const std::string &, Room*, Room*);
    Passage(const std::string &, const std::string &, std::shared_ptr<Command>, Room*, Room*);

    void setFrom(Room*);
    Room* getFrom() const;
    void setTo(Room*);
    Room* getTo() const;

    void setLocked(bool);
    bool isLocked() const;

protected:
    static std::string oppositeDirection(const std::string &);

    Room* fromRoom;
    Room* toRoom;

    bool locked = false;
};

#endif //ZOORK_PASSAGE_H
