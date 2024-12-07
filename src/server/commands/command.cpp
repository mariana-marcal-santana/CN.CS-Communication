#include "command.hpp"

std::string UDPCommand::execute() {
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

    std::string fileName = (std::string)DB_GAMES_PATH + "GAME_" + plid + ".txt";

    try {
        for (const auto &entry : std::filesystem::directory_iterator(DB_GAMES_PATH)) {
            if (entry.path().filename().string() == fileName) {
                std::ifstream file(fileName);
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