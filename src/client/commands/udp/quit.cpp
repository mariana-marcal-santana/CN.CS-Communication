#include "quit.hpp"

void QuitCommand::handleReceive() { // RQT status [C1 C2 C3 C4]

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args.size() != 2 && args.size() != 6) {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    }
    else if (args[0] != RQT) {
        std::cout << WRONG_COMMAND_MSG_SERVER << std::endl;
        return;
    }

    if (args.size() == 2) {
        if (args[1] == NOK) {
            std::cout << "There isn't an ongoing game for this PLID." << std::endl;
        }
        else if (args[1] == ERR) {
            std::cout << INVALID_COMMAND_MSG << std::endl;
        }
    }
    else if (args.size() == 6) {
        if (args[1] == OK) { 
            arg = args[2] + " " + args[3] + " " + args[4] + " " + args[5];
            std::cout << "Game closed... Secret key: " << arg << std::endl;
        } 
    }
}

std::string QuitCommand::formatData() {
    return "QUT " + this->client->plid + "\n";
}