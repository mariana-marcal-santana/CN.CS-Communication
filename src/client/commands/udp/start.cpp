#include "start.hpp"

void StartCommand::receive() { // RSG status

    std::string dataReceived = this->Client->receiveData();

    if (dataReceived.find("OK") != std::string::npos) { 
        std::cout << "New game has started!" << std::endl;
    } 
    else if (dataReceived.find("NOK") != std::string::npos) {
        std::cout << "There's already an ongoing game for this PLID." << std::endl;
    }
    else if (dataReceived.find("ERR") != std::string::npos) {
        std::cout << INVALID_COMMAND_MSG << ": Invalid PLID or incorrect time." << std::endl;
    }
}

std::string StartCommand::formatData() {
    return "SNG " + this->plid + " " + this->max_playtime + "\0";
}
