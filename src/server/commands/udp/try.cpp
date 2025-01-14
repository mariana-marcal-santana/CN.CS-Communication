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

std::string TryCommand::evalTry(std::string solution) {
    std::string guess = this->C1 + this->C2 + this->C3 + this->C4;

    std::vector<bool> codeMatched(solution.size(), false); // Tracks matched code positions
    std::vector<bool> guessMatched(guess.size(), false); // Tracks matched guess positions

    for (size_t i = 0; i < solution.size(); ++i) {
        if (solution[i] == guess[i]) {
            ++this->nB;
            codeMatched[i] = true;
            guessMatched[i] = true;
        }
    }

    for (size_t i = 0; i < solution.size(); ++i) {
        if (!codeMatched[i]) {
            for (size_t j = 0; j < guess.size(); ++j) {
                if (!guessMatched[j] && solution[i] == guess[j]) {
                    ++this->nW;
                    guessMatched[j] = true;
                    break;
                }
            }
        }
    }
    return std::to_string(this->nB) + " " + std::to_string(this->nW);
}

std::string TryCommand::evalLogTry(std::string solution, std::string time) {

    std::string result = evalTry(solution);

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

void TryCommand::logGame(std::string code, std::string colors, std::string mode, int tries, std::time_t now, std::time_t init) {

    this->logGameG(code, now, init);

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "_%d%m%Y_%H%M%S_");

    std::string score = calcScore(tries, now - init);

    std::ofstream file((std::string)DB_SCORES_PATH + "/" + score + "_" + this->plid + timestamp.str() + mode + ".txt");
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

    std::fstream file;
    std::string fileName = (std::string)DB_GAMES_PATH + "/GAME_" + plid + ".txt";
    file.open(fileName, std::ios::in | std::ios::out | std::ios::app);
    if (!file.is_open()) {
        return "RTR NOK\n";
    }

    std::string playerInfo;
    std::getline(file, playerInfo);

    std::istringstream iss(playerInfo);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    std::time_t now = std::time(nullptr);
    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S_");

    // game timeout
    if (now - std::stoi(args[6]) > std::stoi(args[3])) {
        this->logGameG("T", now, std::stoi(args[6]));
        std::string result = "RTR ETM";
        for (size_t i = 0; i < args[2].length(); i++)
            result = result + " " + args[2][i];
        return result + "\n";
    }

    std::vector<std::string> tries;
    std::string line;
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
    
    // too many tries
    if (std::atoi(this->nT.c_str()) > MAX_TRIES) {
        this->logGameG("F", now, std::stoi(args[6]));
        std::string result = "RTR ENT";
        for (size_t i = 0; i < args[2].length(); i++)
            result = result + " " + args[2][i];
        return result + "\n";
    }

    // duplicate or inavlid try
    printf("nT: %d\n", std::stoi(this->nT));

    int nT_expected = static_cast<int>(tries.size() + 1);
    printf("nT_expected: %d\n", nT_expected);
    std::string currentTry = this->C1 + this->C2 + this->C3 + this->C4;

    if (std::stoi(this->nT) == nT_expected - 1) {
        if (currentTry != tries.back()) {
            return "RTR INV\n";
        }
    }
    else if (std::stoi(this->nT) != nT_expected - 1 && std::stoi(this->nT) != nT_expected) {
        return "RTR INV\n";
    }
    else if (std::stoi(this->nT) == nT_expected) {
        for (std::string t : tries) {
            if (t == currentTry) {
                return "RTR DUP\n";
            }
        }
    }

    // valid try
    file.clear();
    file.seekp(0, std::ios::end);

    std::string evalTry = this->evalTry(args[2]);
    std::string write = "T: " + this->C1 + this->C2  + this->C3 + this->C4 + " " + evalTry + " " + std::to_string(now - std::stoi(args[6]));
    printf("write: %s\n", write.c_str());
    file << write << std::endl;

    file.close();

    if (std::atoi(this->nT.c_str()) == MAX_TRIES && strcmp(evalTry.c_str(), "4 0") != 0) {
        this->logGameG("F", now, std::stoi(args[6]));
        std::string result = "RTR ENT";
        for (size_t i = 0; i < args[2].length(); i++)
            result = result + " " + args[2][i];
        return result + "\n";
    }

    if (strcmp(evalTry.c_str(), "4 0") == 0) {
        this->logGame("W", args[2], args[1], tries.size() + 1, now, std::stoi(args[6]));
    }
    
    return "RTR OK " + std::to_string(tries.size() + 1) + " " + evalTry + "\n";
}