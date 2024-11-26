#include "scoreboard.hpp"

void ScoreboardCommand::handleReceive() { // RSS status [Fname Fsize Fdata]

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args[1] == EMPTY) { 
        std::cout << "No game has been won yet." << std::endl;
    } 
    else if (args[1] == OK) {
        std::cout << this->data << std::endl;
    }
}

std::string ScoreboardCommand::formatData() {
    return "SSB\n";
}   