#include "start.hpp"

void StartCommand::handleReceive() { // RSG status

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    if (args.size() == 1 && args[0] == ERR) {
        std::cout << INVALID_COMMAND_MSG << std::endl;
        return;
    }
    
    if (args.size() != 2) {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    }
    if (args[0] != RSG) {
        std::cout << WRONG_COMMAND_MSG_SERVER << std::endl;
        return;
    }
    if (args[1] == NOK) { 
        this->client->playing = true;
        this->client->plid = this->plid;
        std::cout << "There's already an ongoing game for this PLID." << std::endl;
    } 
    else if (args[1] == OK) {
        this->client->playing = true;
        this->client->tries = 1;
        this->client->plid = this->plid;
        std::cout << "New game has started!" << std::endl;
    }
    else if (args[1] == ERR) {
        std::cout << INVALID_COMMAND_MSG << ": Invalid PLID or incorrect time." << std::endl;
    }
    else {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
    }
}

std::string StartCommand::formatData() {
    return "SNG " + this->plid + " " + this->max_playtime + "\n";
}

bool StartCommand::shouldSend() {
    if (this->client->playing && this->client->plid != this->plid) {
        std::cout << "There's already an ongoing game for this client." << std::endl;
        return false;
    }
    return true;
}
