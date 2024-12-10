#include "command.hpp"

std::string UDPCommand::execute() {
    printf("execute");
    if (!this->check())
        return "ERR\n";
    return this->exec();
}

std::string TCPCommand::execute() {
    if (!this->check())
        return "ERR\n";
    return this->exec();
}

std::string Command::findPlayerInfo(std::string plid) { 

    std::string fileName = "GAME_" + plid + ".txt";

    try {
        for (const auto &entry : std::filesystem::directory_iterator(DB_GAMES_PATH)) {
            if (entry.path().filename().string() == fileName) {
                // printf("found");
                // printf("filename: %s", ((std::string)DB_GAMES_PATH + "/" + fileName).c_str());
                std::ifstream file((std::string)DB_GAMES_PATH + "/" + fileName);
                if (!file.is_open()) {
                    std::cerr << "Unable to open file." << std::endl;
                    exit(1);
                }
                std::string line;
                std::getline(file, line);
                file.close();
                return line;
            }
        }
    } catch (const std::filesystem::filesystem_error &err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }
    return "";
}

int Command::getPlayerTries(std::string plid) {
    std::ifstream file("players.txt");
    if (!file.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string arg;
        if (arg == plid) {
            iss >> arg;
            return std::stoi(arg);
        }
    }
    file.close();
    return 0;
}


int Command::createPlayerFile(std::string plid, char mode, std::string key, int time){
    std::string playerInfo;
    std::string filename = "GAME_" + plid + ".txt";
    std::string filepath = (std::string)DB_GAMES_PATH + "/" + filename; 

    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }

    std::time_t sec = std::time(nullptr);
    std::tm* localTime = std::localtime(&sec);

    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

    playerInfo = plid + " " + mode + " " + key + " " + std::to_string(time) + " " + ss.str() + " " + std::to_string(sec) + "\n";
    
    file << playerInfo;
    file.close();
    return 0;
}