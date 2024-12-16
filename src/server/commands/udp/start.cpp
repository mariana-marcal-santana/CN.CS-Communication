#include "start.hpp"

bool StartCommand::check() {
    return this->plid.length() == 6 && std::all_of(this->plid.begin(), this->plid.end(), ::isdigit) 
    && this->max_playtime < 601 && std::to_string(this->max_playtime).size() == 3;
}

std::string StartCommand::generateColorKey() {
    const std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    std::string key;

    std::srand(std::time(0)); 

    for (int i = 0; i < 4; i++){
        int idx = std::rand() % colors.size();
        key += colors[idx];
    }
    return key;
}

std::string StartCommand::exec() {

    std::string playerInfo = this->findPlayerInfo(this->plid);
    
    if (playerInfo != "")
        return "RSG NOK\n";

    std::string key = generateColorKey();
    createPlayerFile(this->plid, 'P', key, this->max_playtime);

    return "RSG OK\n";
}
