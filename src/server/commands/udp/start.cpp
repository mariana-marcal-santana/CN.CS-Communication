#include "start.hpp"

std::string StartCommand::genColors() {
    std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<size_t> dist(0, colors.size() - 1);

    std::string result;
    for (size_t i = 0; i < 4; ++i) {
        result += colors[dist(rng)];
    }

    return result;
}

bool StartCommand::check() {
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    if (!std::all_of(this->max_playtime.begin(), this->max_playtime.end(), ::isdigit) ||
        std::stoi(this->max_playtime) < 1 || std::stoi(this->max_playtime) > GAME_TIMEOUT) {
        return false;
    }
    return true;
}

std::string StartCommand::exec() {
    // playerInfo: plid mode colors time date hour timestamp
    std::string playerInfo = this->findPlayerInfo(this->plid);
    
    // active game
    if (playerInfo != "") {
        std::istringstream iss(playerInfo);
        std::string arg;
        std::vector<std::string> args;
        while (iss >> arg) { args.push_back(arg); }

        time_t now = std::time(nullptr);
        // game hasn't timed out but has tries
        if (now - std::stoi(args[6]) <= std::stoi(args[3])) {
            if (getPlayerTries(this->plid).size() > 0)
                return "RSG NOK\n";
        }
        else { // game timeout - log game and make new one
            this->logGameG("T", now, std::stoi(args[6]));
        }
    }

    std::ofstream file((std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt");
    if (!file.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }

    std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime = *std::localtime(&currentTime);
    std::ostringstream timeStream;
    timeStream << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

    file << this->plid + " P " + this->genColors() + " " + this->max_playtime + 
        " " + timeStream.str() + " " + std::to_string(std::time(nullptr)) << std::endl;

    file.close();

    return "RSG OK\n";
}