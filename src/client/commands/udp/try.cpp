#include "try.hpp"

void TryCommand::receive() { // RTR status [nT nB nW][C1 C2 C3 C4]

    std::string dataReceived = this->Client->receiveData();

    std::istringstream iss(dataReceived);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args[1] == OK) { 
        arg = args[2] + " " + args[3] + " " + args[4];
        std::cout << "Valid try: " << arg << std::endl;
        if (args[3].find("4") != std::string::npos) {
            std::cout << "Game won!" << std::endl;
        }
        else { this->Client->tries ++; }
    }
    else if (args[1] == DUP) {
        std::cout << "Duplicate try." << std::endl;
    }
    else if (args[1] == INV) {
        // ?????
    }
    else if (args[1] == NOK) {
        std::cout << "Try out of context." << std::endl;
    }
    else if (args[1] == ENT) {
        arg = args[2] + " " + args[3] + " " + args[4] + " " + args[5];
        std::cout << "Game lost... Secret key: " << arg << std::endl;
    }
    else if (args[1] == ETM) {
        arg = args[2] + " " + args[3] + " " + args[4] + " " + args[5];
        std::cout << "Time is up... Secret key: " << arg << std::endl;
    }
    else if (args[1] == ERR) {
        std::cout << "Incorrect try (check for invalid PLID our colour codes)." << std::endl;
    }
}

std::string TryCommand::formatData() {
    return "TRY " + std::to_string(this->Client->plid) + " " + this->C1 + " " + this->C2 + 
        " " + this->C3 + " " + this->C4 + " " + std::to_string(this->Client->tries) + "\0";
}