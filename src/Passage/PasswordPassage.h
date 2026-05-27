#ifndef ZOORK_PASSWORDPASSAGE_H
#define ZOORK_PASSWORDPASSAGE_H

#include "Passage.h"

class PasswordPassage : public Passage {
public:
    PasswordPassage(
        const std::string& name,
        const std::string& description,
        Room* from,
        Room* to,
        const std::string& password
    );

    void setPassword(std::string);
    bool tryUnlock(const std::string& inputPassword);

private:
    std::string password;
};

#endif