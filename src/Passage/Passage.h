#ifndef ZOORK_PASSAGE_H
#define ZOORK_PASSAGE_H

#include "Room/NullRoom.h"
#include "Room/Room.h"
#include <iostream>

class Passage : public Location {
public:
    template<typename T, typename... Args>
    static std::vector<std::shared_ptr<Passage>> createBasicPassage(
        Room* from,
        Room* to,
        const std::string& direction,
        bool locked,
        bool bidirectional,
        Args&&... args
    );
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

template<typename T, typename... Args>
std::vector<std::shared_ptr<Passage>> Passage::createBasicPassage(Room* from, Room* to, const std::string& direction, bool locked,bool bidirectional, Args&&... args) {
    std::string passageName = from->getName() + "_to_" + to->getName();

    auto temp1 = std::make_shared<T>(passageName, "A totally normal passageway.", from, to, std::forward<Args>(args)...);
    from->addPassage(direction, temp1);
    temp1->setLocked(locked);

    if (bidirectional) {
        std::string passageName2 = to->getName() + "_to_" + from->getName();
        auto temp2 = std::make_shared<T>(passageName2, "A totally normal passageway.", to,from, std::forward<Args>(args)...);

        to->addPassage(oppositeDirection(direction), temp2);
        temp2->setLocked(locked);

        return {temp1, temp2};
    }

    return {temp1};
}

#endif //ZOORK_PASSAGE_H
