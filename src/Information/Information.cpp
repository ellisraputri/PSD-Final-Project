#include "Information.h"

Information* Information::infoInstance = nullptr;

Information* Information::instance() {
    if (!infoInstance) {
        infoInstance = new Information();
    }
    return infoInstance;
}

std::shared_ptr<Item> Information::getItem(std::string name) {
    auto it = allItems.find(name);
    if (it != allItems.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Mechanism> Information::getMechanism(std::string name) {
    auto it = allMechanisms.find(name);
    if (it != allMechanisms.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Room> Information::getRoom(std::string name) {
    auto it = allRooms.find(name);
    if (it != allRooms.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Character> Information::getCharacter(std::string name) {
    auto it = allCharacters.find(name);
    if (it != allCharacters.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Passage> Information::getPassage(std::string name) {
    auto it = allPassages.find(name);
    if (it != allPassages.end()) {
        return it->second;
    }
    return nullptr;
}

void Information::setItem(const std::string& name, std::shared_ptr<Item> item) {
    allItems[name] = item;
}

void Information::setMechanism(const std::string& name, std::shared_ptr<Mechanism> mechanism) {
    allMechanisms[name] = mechanism;
}

void Information::setRoom(const std::string& name, std::shared_ptr<Room> room) {
    allRooms[name] = room;
}

void Information::setPassage(const std::string& name, std::shared_ptr<Passage> passage) {
    allPassages[name] = passage;
}

void Information::setCharacter(const std::string& name, std::shared_ptr<Character> character) {
    allCharacters[name] = character;
}

json Information::readJson(std::string loc) {
    std::ifstream file(loc);
    if (!file.is_open()) {
        std::cout << "cannot open " << loc << std::endl;
        throw std::runtime_error("Cannot open file");
    }

    json j;
    file >> j;

    return j;
}

void Information::initRoom() {
    json j = readJson("data/room.json");

    for (const auto& data : j["rooms"]) {
        RoomConfig config = RoomConfig::fromJson(data);
        std::shared_ptr<Room> room = std::make_shared<Room>(config.name, config.description);

        allRooms[config.name] = room;
    }
}

std::vector<std::shared_ptr<Passage>> Information::createPassage(PassageConfig& config) {
    if (!config.password.empty()) {
        return Passage::createBasicPassage<PasswordPassage>(
            allRooms[config.fromRoom].get(),
            allRooms[config.toRoom].get(),
            config.direction,
            config.locked,
            config.bidirectional,
            config.password
        );
    }
    else {
        return Passage::createBasicPassage<Passage>(
            allRooms[config.fromRoom].get(),
            allRooms[config.toRoom].get(),
            config.direction,
            config.locked,
            config.bidirectional
        );
    }
}

void Information::initPassage() {
    json j = readJson("data/passage.json");

    for(const auto& data: j["passages"]){
        PassageConfig config = PassageConfig::fromJson(data);
        std::vector<std::shared_ptr<Passage>> passages = createPassage(config);
        
        allPassages[passages[0]->getName()] = passages[0];
        if (passages.size() > 1) {
            allPassages[passages[1]->getName()] = passages[1];
        }
    }
}

void Information::initMechanism() {
    json j = readJson("data/mechanism.json");

    for(const auto& data: j["mechanisms"]){
        MechanismConfig config = MechanismConfig::fromJson(data);

        auto mechanism = std::make_shared<Mechanism>(config.name, config.description, config.print);
        allMechanisms[config.name] = mechanism;
    }
}

std::shared_ptr<BuffItem> Information::createBuffItem(ItemConfig& config) {
    std::shared_ptr<BuffItem> item = std::make_shared<BuffItem>(
        config.name, config.description, config.hpBuff, config.atkBuff, config.defBuff
    );
    
    auto buffCommand = std::make_shared<BuffCharacterItemCommand>(
        item.get(), item.get()
    );
    item->setUseCommand(buffCommand);

    return item;
}

std::shared_ptr<Item> Information::createUnlockPassageItem(ItemConfig& config) {
    std::shared_ptr<Item> item = std::make_shared<Item>(config.name, config.description);
    
    auto unlockCommand = std::make_shared<PassageDefaultUnlockCommand>(
        allPassages[config.passage1].get(),
        allPassages[config.passage2].get(),
        item.get()
    );
    item->setUseCommand(unlockCommand);

    return item;
}

std::shared_ptr<Item> Information::createUnlockMechanismItem(ItemConfig& config) {
    std::shared_ptr<Item> item = std::make_shared<Item>(config.name, config.description);
    
    auto unlockCommand = std::make_shared<MechanismUnlockCommand>(
        allMechanisms[config.mechanism].get(),
        item.get()
    );
    item->setUseCommand(unlockCommand);

    return item;
}

std::shared_ptr<Item> Information::createDecorativeItem(ItemConfig& config) {
    std::shared_ptr<Item> item = std::make_shared<Item>(config.name, config.description);
    item->setTakeable(false);
    return item;
}

void Information::initItem() {
    json j = readJson("data/item.json");

    for(const auto& data: j["items"]){
        ItemConfig config = ItemConfig::fromJson(data);
        std::shared_ptr<Item> item;

        if (config.type == "buff") {
            item = createBuffItem(config);
        } 
        else if (config.type == "unlock-passage"){
            item = createUnlockPassageItem(config);
        }
        else if (config.type == "unlock-mechanism"){
            item = createUnlockMechanismItem(config);
        }
        else {
            item = createDecorativeItem(config);
        }
        
        allItems[config.name] = item;
    }
}

void Information::initCharacter(){
    json j = readJson("data/character.json");

    for(const auto& data: j["characters"]){
        CharacterConfig config = CharacterConfig::fromJson(data);
        std::shared_ptr<Character> character;

        if (config.combatMode){
            character = std::make_shared<CombatCharacter>(
                config.name, config.description, config.hp, config.atk, config.def
            );
        } 
        else {
            character = std::make_shared<Character>(config.name, config.description);
        }

        for (auto& [flag, text] : config.dialogues) {
            character->addDialogue(flag, text);
        }

        allCharacters[config.name] = character;
    }
}

void Information::initRoomPopulation() {
    json j = readJson("data/room.json");

    for (const auto& data : j["rooms"]) {
        RoomConfig config = RoomConfig::fromJson(data);
        auto room = allRooms[config.name];

        for (const auto& itemName : config.items) {
            room->addItem(allItems[itemName]);
        }
        for (const auto& characterName : config.characters) {
            room->addCharacter(allCharacters[characterName]);
        }
        for (const auto& mechanismName : config.mechanisms) {
            room->addMechanism(allMechanisms[mechanismName]);
        }
    }
}

void Information::initTrigger() {
    json j = readJson("data/trigger.json");

    for (const auto& data: j["triggers"]){
        TriggerConfig config = TriggerConfig::fromJson(data);

        StoryTrigger trigger = StoryTrigger(stringToTriggerType(config.triggerType), config.target, config.flag, config.result, config.flagDone, config.flagCondition, config.lockPlayer);
        StoryManager::instance()->addTrigger(trigger);
    }
}

void Information::initCheckpoints(){
    json j = readJson("data/checkpoint.json");

    for (const auto& data: j["checkpoints"]) {
        checkpoints.push_back(allRooms[data]);
    }
}

std::vector<std::string> Information::getCheckpointList(std::string current) {
    if (current == checkpoints[0]->getName()) {
        return std::vector<std::string>();
    }

    std::vector<std::string> returnList;
    int loopSize = 0;

    for (int i=0; i<checkpoints.size(); i++){
        if (current == checkpoints[i]->getName()){
            loopSize = i;
        }
    }

    loopSize = (loopSize % 2 ==1)? loopSize+1 : loopSize;
    for (int i=0; i<=loopSize; i++){
        returnList.push_back(checkpoints[i]->getName());
    }

    return returnList;
}

void Information::initAll() {
    initRoom();
    initPassage();
    initMechanism();
    initItem();
    initCharacter();
    initRoomPopulation();
    initTrigger();
    initCheckpoints();
}