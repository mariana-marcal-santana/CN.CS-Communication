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

                std::ifstream file((std::string)DB_GAMES_PATH + "/" + fileName);
                if (!file.is_open()) {
                    perror("Unable to open file.");
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
