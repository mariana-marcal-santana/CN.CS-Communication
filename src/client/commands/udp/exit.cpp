#include "exit.hpp"

void ExitCommand::receive() { // RQT status [C1 C2 C3 C4]

    std::string dataReceived = this->Client->receiveData();

    std::istringstream iss(dataReceived);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args[1] == OK) { 
        arg = args[2] + " " + args[3] + " " + args[4] + " " + args[5];
        std::cout << "Game ended... Secret key: " << arg << std::endl;
    } 
    else if (args[1] == NOK) {
        std::cout << "There isn't an ongoing game for this PLID." << std::endl;
    }
    else if (args[1] == ERR) {
        std::cout << INVALID_COMMAND_MSG << std::endl;
    }
}

std::string ExitCommand::formatData() {
    return "QUT " + std::to_string(this->Client->plid) + "\0";
}   