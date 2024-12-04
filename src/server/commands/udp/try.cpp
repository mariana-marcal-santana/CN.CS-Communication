#include "try.hpp"

std::string TryCommand::evalTry(std::string solution) {
    std::string guess = this->C1 + this->C2 + this->C3 + this->C4;
    for (size_t i = 0; i < solution.length(); i++) {
        if (solution[i] == guess[i]) 
            this->nB ++;
        else if (solution.find(guess[i]) != std::string::npos)
            this->nW ++;
    }
    return std::to_string(this->nB) + std::to_string(this->nW);
}

std::vector<std::string> TryCommand::getPlayerTries(std::string plid) {
    std::ifstream file("GAME_" + plid + ".txt");
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    std::string line;
    std::vector<std::string> tries;
    while (std::getline(file, line)) {
        if (line[0] == 'T') {
            std::istringstream iss(line);
            std::string arg;
            iss >> arg;
            tries.push_back(arg);
        }
    }
    file.close();
    return tries;
}

bool TryCommand::check() {
    std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    if (std::find(colors.begin(), colors.end(), this->C1) == colors.end() ||
        std::find(colors.begin(), colors.end(), this->C2) == colors.end() ||
        std::find(colors.begin(), colors.end(), this->C3) == colors.end() ||
        std::find(colors.begin(), colors.end(), this->C4) == colors.end()) {
        return false;
    }
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    if (!std::all_of(this->nT.begin(), this->nT.end(), ::isdigit)) {
        return false;
    }
    return true;
}

std::string TryCommand::exec() {

    // playerInfo: plid mode colors time date hour timestamp
    std::string playerInfo = this->findPlayerInfo(this->plid);
    
    // no active game
    if (playerInfo == "")
        return "RTR NOK\n";

    std::istringstream iss(playerInfo);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    // game timeout
    if (std::time(nullptr) - std::stoi(args[6]) > std::stoi(args[3]))
        return "RTR ETM\n";

    std::vector<std::string> tries = this->getPlayerTries(this->plid);

    // too many tries
    if (tries.size() > MAX_TRIES) {
        std::string result = "RTR ENT";
        for (size_t i = 0; i < args[2].length(); i++)
            result = result + " " + args[i];
        return result + "\n";
    }

    // duplicate try
    for (std::string t : tries) {
        if (t == this->C1 + this->C2 + this->C3 + this->C4)
            return "RTR DUP\n";
    }

    // invalid try number
    if (std::stoi(this->nT) != tries.size() + 1)
        return "RTR INV\n";

    // valid try
    std::string evalTry = this->evalTry(args[2]);
    std::ofstream file("GAME_" + this->plid + ".txt");
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    file << "T: " + this->C1 + " " + this->C2 + " " + this->C3 + " " + this->C4 + " " + evalTry +
        std::to_string(std::time(nullptr) - std::stoi(args[6])) + "\n" << std::endl;
    file.close();
    
    return "RTR OK " + std::to_string(tries.size() + 1) + evalTry + "\n";
}