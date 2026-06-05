#ifndef ENGINEUTILS_H
#define ENGINEUTILS_H

#include <vector> 
#include <string> 
#include <functional> 
#include <utility> 
#include <algorithm> 
#include <memory> 
#include <random> 
#include <iostream> 
#include <sstream> 
#include <limits>

class EngineUtils {
public:
    static std::vector<std::string> tokenizeString(const std::string&);

    static std::string makeLowercase(std::string);

    static int randomInt(int min, int max);

    static bool processLookTargets(
        const std::vector<std::string>& arguments,
        std::vector<bool>& isPrinted,
        std::function<bool(const std::string&)>,
        std::function<void(const std::string&)>
    );

    static int calculateDamage(int atk, int def);

    static void printHelpCommand();
};

#endif