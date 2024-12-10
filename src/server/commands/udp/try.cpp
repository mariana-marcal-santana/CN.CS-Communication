#include "try.hpp"

std::string TryCommand::calcScore(int tries, int seconds) {

    double triesFactor = std::min(1.0, static_cast<double>(MAX_TRIES - tries) / MAX_TRIES);
    double timeFactor = std::min(1.0, static_cast<double>(GAME_TIMEOUT - seconds) / GAME_TIMEOUT);
    int score = static_cast<int>(std::round((triesFactor * 0.5 + timeFactor * 0.5) * 100));

    std::ostringstream oss;
    oss.width(3);
    oss.fill('0');
    oss << score;    

    return oss.str();
}

std::string TryCommand::evalLogTry(std::string solution, std::string time) {
    printf("evalLogTry");
    std::string guess = this->C1 + this->C2 + this->C3 + this->C4;
    for (size_t i = 0; i < solution.length(); i++) {
        if (solution[i] == guess[i]) 
            this->nB ++;
        else if (solution.find(guess[i]) != std::string::npos)
            this->nW ++;
    }
    std::string result = std::to_string(this->nB) + " " + std::to_string(this->nW);

    // log try
    std::ofstream file((std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt", std::ofstream::app);
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    file << "T: " + this->C1 + this->C2  + this->C3 + this->C4 + " " + result + " " + time << std::endl;
    file.close();

    return result;
}

std::vector<std::string> TryCommand::getPlayerTries(std::string plid) {
    printf("getPlayerTries");
    std::ifstream file((std::string)DB_GAMES_PATH + "/GAME_" + plid + ".txt");
    if (!file) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    std::string line;
    std::vector<std::string> tries;
    while (std::getline(file, line)) {
        if (line[0] == 'T') {
            std::istringstream iss(line);
            std::vector<std::string> args;
            std::string arg;
            while (iss >> arg) {
                args.push_back(arg);
            }
            tries.push_back(args[1]);
        }
    }
    file.close();
    return tries;
}

void TryCommand::logGame(std::string code, std::time_t now, std::time_t init) {

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S_");

    std::string srcFileName = (std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt";
    std::ifstream src(srcFileName);

    std::filesystem::create_directories((std::string)DB_GAMES_PATH + "/" + this->plid);
    std::ofstream dst((std::string)DB_GAMES_PATH + "/" + this->plid + "/" + timestamp.str() + code + ".txt");

    if (!src.is_open() || !dst.is_open()) {
        std::perror("Error opening files");
        exit(1);
    }

    std::string line;
    while (std::getline(src, line)) {
        dst << line << std::endl;
    }

    timestamp.str("");
    timestamp.clear(); 
    timestamp << std::put_time(std::localtime(&now), "%d-%m-%Y %H:%M:%S");
    dst << timestamp.str() + " " + std::to_string(now - init) << std::endl;

    src.close();
    dst.close();

    if (std::remove(srcFileName.c_str()) == ERROR) {
        std::perror("Error deleting file");
    }
}

void TryCommand::logGame(std::string code, std::string colors, std::string mode, int tries, std::time_t now, std::time_t init) {

    this->logGame(code, now, init);

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "_%d%m%Y_%H%M%S_");

    std::string score = calcScore(tries, now - init);

    std::ofstream file((std::string)DB_SCORES_PATH + "/" + score + "_" + this->plid + timestamp.str() + ".txt");
    if (!file.is_open()) {
        printf("file score");
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
    printf("execTry");
    // playerInfo: plid mode colors time date hour timestamp
    std::string playerInfo = this->findPlayerInfo(this->plid);
    
    // no active game
    if (playerInfo == "") {
        printf("no active game");
        return "RTR NOK\n";
    }

    std::istringstream iss(playerInfo);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    std::time_t now = std::time(nullptr);
    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S_");

    // game timeout
    if (now - std::stoi(args[6]) > std::stoi(args[3])) {
        printf("timeout");
        this->logGame("T", now, std::stoi(args[6]));
        std::string result = "RTR ETM";
        for (size_t i = 0; i < args[2].length(); i++)
            result = result + " " + args[i];
        return result + "\n";
    }

    std::vector<std::string> tries = this->getPlayerTries(this->plid);

    // too many tries
    if (std::atoi(this->nT.c_str()) > MAX_TRIES) {
        printf("too many tries");
        this->logGame("F", now, std::stoi(args[6]));
        std::string result = "RTR ENT";
        for (size_t i = 0; i < args[2].length(); i++)
            result = result + " " + args[i];
        return result + "\n";
    }

    // duplicate try
    for (std::string t : tries) {
        if (t == this->C1 + this->C2 + this->C3 + this->C4) {
            return "RTR DUP\n";
        }
    }

    // invalid try number
    if (std::stoi(this->nT) != static_cast<int>(tries.size() + 1)) {
        printf("invalid try number");
        return "RTR INV\n";
    }

    // valid try
    std::string evalLogTry = this->evalLogTry(args[2], std::to_string(now - std::stoi(args[6])));
    if (evalLogTry == "4 0") {
        this->logGame("W", args[2], args[1], tries.size() + 1, now, std::stoi(args[6]));
    }
    
    return "RTR OK " + std::to_string(tries.size() + 1) + " " + evalLogTry + "\n";
}