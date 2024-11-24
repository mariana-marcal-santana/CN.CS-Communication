#include "showtrials.hpp"

void ShowTrialsCommand::handleReceive() { // RST status [Fname Fsize Fdata]

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args[1] == ACT) { 
        std::cout << "Process file TODO" << std::endl;
    } 
    else if (args[1] == FIN) {
        std::cout << "Process file TODO" << std::endl;
    }
    else if (args[1] == NOK) {
        std::cout << "There're no games for this player." << std::endl;
    }
}

std::string ShowTrialsCommand::formatData() {
    return "STR " + this->client->plid + "\n";
}   