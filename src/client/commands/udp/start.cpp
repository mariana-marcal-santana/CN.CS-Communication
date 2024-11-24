#include "start.hpp"

void StartCommand::handleReceive() { // RSG status

    printf("Data received: %s\n", this->data.c_str());

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
