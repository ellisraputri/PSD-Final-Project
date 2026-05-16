#ifndef ZOORK_ROOM_H
#define ZOORK_ROOM_H

#include "Character/Character.h"
#include "Command/RoomDefaultEnterCommand.h"
#include "Item/Item.h"
#include "Location.h"
#include <map>

class Passage;

class Room : public Location {
public:
    Room(const std::string &, const std::string &);

    Room(const std::string &, const std::string &, std::shared_ptr<Command>);

    std::string getDescription() const override;

    void addItem(std::shared_ptr<Item>);
    void removeItem(std::shared_ptr<Item>);
    bool isItemExist(const std::shared_ptr<Item>&);

//    void addCharacter(Character*);
//    void removeCharacter(const std::string&);
//    Character* getCharacter(const std::string&);

    void addPassage(const std::string &, std::shared_ptr<Passage>);

    void removePassage(const std::string &);

    std::shared_ptr<Passage> getPassage(const std::string &);

protected:
    std::vector<std::shared_ptr<Item>> items;
//    std::vector<Character*> characters;
    std::map<std::string, std::shared_ptr<Passage>> passageMap;
};


#endif //ZOORK_ROOM_H
