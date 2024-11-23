#include "scoreboard.hpp"

void ScoreboardCommand::receive() { // RSS status [Fname Fsize Fdata]

    std::string dataReceived = this->client->receiveData();

    std::istringstream iss(dataReceived);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args[1] == EMPTY) { 
        std::cout << "No game has been won yet." << std::endl;
    } 
    else if (args[1] == OK) {
        std::cout << "Process file TODO" << std::endl;
    }
}

std::string ScoreboardCommand::formatData() {
    return "SSB\0";
}   