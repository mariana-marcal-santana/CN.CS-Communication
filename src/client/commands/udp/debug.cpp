#include "debug.hpp"

void DebugCommand::handleReceive() { // RDB status

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args.size() == 1 && args[0] == ERR) {
        std::cout << INVALID_COMMAND_MSG << std::endl;
        return;
    }

    if (args.size() != 2) {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    }

    if (args[0] != RDB) {
        std::cout << WRONG_COMMAND_MSG_SERVER << std::endl;
        return;
    }

    if (args[1] == OK) { 
        this->client->playing = true;
        std::cout << "Debug game has started!" << std::endl;
    } 
    else if (args[1] == NOK) {
        this->client->playing = true;
        std::cout << "There's an ongoing game for this PLID." << std::endl;
    }
    else if (args[1] == ERR) {
        std::cout << INVALID_COMMAND_MSG << ": (check for invalid PLID our colour codes)." << std::endl;
    }
}

std::string DebugCommand::formatData() {
    return "DBG " + this->plid + " " + this->max_playtime + " " + 
        this->C1 + " " + this->C2 + " " + this->C3 + " " + this->C4 + "\n";
} 

bool DebugCommand::shouldSend() {
    bool res = this->client->plid != "";
    if (!res) {
        std::cout << "You must set a plid." << std::endl;
    }
    return res;
}