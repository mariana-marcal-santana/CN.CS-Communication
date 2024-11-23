#include "debug.hpp"

void DebugCommand::receive() { // RDB status

    std::string dataReceived = this->client->receiveData();

    std::istringstream iss(dataReceived);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args[1] == OK) { 
        std::cout << "Debug game has started!" << arg << std::endl;
    } 
    else if (args[1] == NOK) {
        std::cout << "There's an ongoing game for this PLID." << std::endl;
    }
    else if (args[1] == ERR) {
        std::cout << INVALID_COMMAND_MSG << ": (check for invalid PLID our colour codes)." << std::endl;
    }
}

std::string DebugCommand::formatData() {
    return "DBG " + this->plid + " " + this->max_playtime + " " + 
        this->C1 + " " + this->C2 + " " + this->C3 + " " + this->C4 + "\0";
}   