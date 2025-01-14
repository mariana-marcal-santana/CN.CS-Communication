#include "try.hpp"

void TryCommand::handleReceive() { // RTR status [nT nB nW][C1 C2 C3 C4]

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    if (args.size() == 1 && args[0] == ERR) {
        std::cout << INVALID_COMMAND_MSG << std::endl;
        return;
    }

    if (args.size() != 2 && args.size() != 5 && args.size() != 6) {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    }

    if (args[0] != RTR) {
        std::cout << WRONG_COMMAND_MSG_SERVER << std::endl;
        return;
    }

    if (args.size() == 2) {
        if (args[1] == DUP) {
            std::cout << "Duplicate try." << std::endl;
        }
        else if (args[1] == INV) {
            std::cout << "Invalid try number." << std::endl;
        }
        else if (args[1] == NOK) {
            std::cout << "Try out of context." << std::endl;
        }
        else if (args[1] == ERR) {
            std::cout << "Incorrect try (check for invalid PLID our colour codes)." << std::endl;
        }
    }

    else if (args.size() == 5) {
        if (args[1] == OK) {
            if (std::stoi(args[2]) != this->client->tries) {
                std::cout << "Wrong number of tries from server." << std::endl;
                return;
            }
            else if (std::stoi(args[3]) < 0 || std::stoi(args[3]) > 4 ||
                std::stoi(args[4]) < 0 || std::stoi(args[4]) > 4) {
                std::cout << "Impossible number(s) of blacks and whites from server." << std::endl;
                return;
            }

            arg = args[2] + "T " + args[3] + "B " + args[4] + "W";
            std::cout << "Valid try: " << arg << std::endl;
            if (args[3].find("4") != std::string::npos) {
                this->client->playing = false;
                std::cout << "Game won!" << std::endl;
            }
            else { this->client->tries ++; }
        }
    }

    else if (args.size() == 6) {
        if (args[1] == ENT) {
            this->client->playing = false;
            arg = args[2] + " " + args[3] + " " + args[4] + " " + args[5];
            std::cout << "Game lost... Secret key: " << arg << std::endl;
        }
        else if (args[1] == ETM) {
            this->client->playing = false;
            arg = args[2] + " " + args[3] + " " + args[4] + " " + args[5];
            std::cout << "Time is up... Secret key: " << arg << std::endl;
        }
    }

    else {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
    }
}

std::string TryCommand::formatData() {
    return "TRY " + this->client->plid + " " + this->C1 + " " + this->C2 + 
        " " + this->C3 + " " + this->C4 + " " + std::to_string(this->client->tries) + "\n";
}

bool TryCommand::shouldSend() {
    bool res = this->client->playing && this->client->plid != "";
    if (!res) { 
        std::cout << "No active game or empty plid." << std::endl; 
    }
    return res;
}