#ifndef ZOORK_BUFFITEM_H
#define ZOORK_BUFFITEM_H

#include "Item.h"

class BuffItem : public Item {
public:
    BuffItem(const std::string& name, const std::string& desc, int hpBuff, int atkBuff, int defBuff)
        : Item(name, desc), hpBuff(hpBuff), atkBuff(atkBuff), defBuff(defBuff) {}

    int getHpBuff();
    int getAtkBuff();
    int getDefBuff();
    
private:
    int hpBuff;
    int atkBuff;
    int defBuff;
};

#endif //ZOORK_BUFFITEM_H