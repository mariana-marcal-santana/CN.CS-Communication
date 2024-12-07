#include "try.hpp"

std::string TryCommand::calcScore(int tries, int seconds) {

    double triesFactor = std::min(1.0, static_cast<double>(MAX_TRIES - tries) / MAX_TRIES);
    double timeFactor = std::min(1.0, static_cast<double>(GAME_TIMEOUT - seconds) / GAME_TIMEOUT);
    double score = (triesFactor * 0.5 + timeFactor * 0.5) * 100;

    return std::to_string(score);
}

std::string TryCommand::evalTry(std::string solution, std::string time) {
    std::string guess = this->C1 + this->C2 + this->C3 + this->C4;
    for (size_t i = 0; i < solution.length(); i++) {
        if (solution[i] == guess[i]) 
            this->nB ++;
        else if (solution.find(guess[i]) != std::string::npos)
            this->nW ++;
    }
    std::string result = std::to_string(this->nB) + std::to_string(this->nW);

    // log try
    std::ofstream file((std::string)DB_GAMES_PATH + "GAME_" + this->plid + ".txt");
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    file << "T: " + this->C1 + " " + this->C2 + " " + this->C3 + " " + this->C4 + " " + result + " " + time << std::endl;
    file.close();

    return result;
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

void TryCommand::logGame(std::string code, std::time_t now) {

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S_");

    std::string srcFileName = (std::string)DB_GAMES_PATH + "GAME_" + this->plid + ".txt";
    std::ifstream src(srcFileName);
    std::ofstream dst((std::string)DB_GAMES_PATH + "/" + this->plid + timestamp.str() + code + ".txt");

    if (!src.is_open() || !dst.is_open()) {
        std::perror("Error opening files");
        exit(1);
    }

    std::string line;
    while (std::getline(src, line)) {
        dst << line << std::endl;
    }

    src.close();
    dst.close();

    if (std::remove(srcFileName.c_str()) == ERROR) {
        std::perror("Error deleting file");
    }
}

void TryCommand::logGame(std::string code, std::string colors, std::string mode, int tries, std::time_t time) {

    this->logGame(code, time);

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&time), "_%d%m%Y_%H%M%S_");

    std::string score = calcScore(this->getPlayerTries(this->plid).size(), time);

    std::ofstream file((std::string)DB_SCORES_PATH + score + "_" + this->plid + timestamp.str() + ".txt");
    if (!file.is_open()) {
        std::perror("Unable to open file");
        exit(1);
    }

    file << score + " " + this->plid + " " + colors + " " + std::to_string(tries) + " " + mode << std::endl;

    file.close();
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

    std::time_t now = std::time(nullptr);
    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S_");

    // game timeout
    if (now - std::stoi(args[6]) > std::stoi(args[3])) {
        this->logGame("T", now - std::stoi(args[6]));
        return "RTR ETM\n";
    }

    std::vector<std::string> tries = this->getPlayerTries(this->plid);

    // too many tries
    if (tries.size() > MAX_TRIES) {
        this->logGame("F", now - std::stoi(args[6]));
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
    if (std::stoi(this->nT) != static_cast<int>(tries.size() + 1))
        return "RTR INV\n";

    // valid try
    std::string evalTry = this->evalTry(args[2], std::to_string(now - std::stoi(args[6])));
    if (evalTry == "4 0")
        this->logGame("W", args[2], args[1], tries.size() + 1, now - std::stoi(args[6]));
    
    return "RTR OK " + std::to_string(tries.size() + 1) + evalTry + "\n";
}