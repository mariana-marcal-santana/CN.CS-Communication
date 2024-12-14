#include "start.hpp"

void StartCommand::handleReceive() { // RSG status

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }
    
    if (args.size() != 2) {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    }
    if (args[0] != RSG) {
        std::cout << WRONG_COMMAND_MSG_SERVER << std::endl;
        return;
    }
    if (this->data.find("NOK") != std::string::npos) { 
        std::cout << "There's already an ongoing game for this PLID." << std::endl;
    } 
    else if (this->data.find("OK") != std::string::npos) {
        std::cout << "New game has started!" << std::endl;
    }
    else if (this->data.find("ERR") != std::string::npos) {
        std::cout << INVALID_COMMAND_MSG << ": Invalid PLID or incorrect time." << std::endl;
    }
}

std::string StartCommand::formatData() {
    return "SNG " + this->plid + " " + this->max_playtime + "\n";
}
