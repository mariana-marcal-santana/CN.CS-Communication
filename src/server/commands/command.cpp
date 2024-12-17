#include "command.hpp"

std::string UDPCommand::execute() {
    if (!this->check())
        return this->cmd_response + " ERR\n";
    return this->exec();
}

std::string TCPCommand::execute() {
    if (!this->check())
        return this->cmd_response + " ERR\n";
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

std::vector<std::string> Command::getPlayerTries(std::string plid) {

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

void Command::logGame(std::string code, std::time_t now, std::time_t init) {

    std::ostringstream timestamp;
    timestamp << std::put_time(std::localtime(&now), "%Y%m%d_%H%M%S_");

    std::string srcFileName = (std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt";
    std::ifstream src(srcFileName);

    std::filesystem::create_directories((std::string)DB_GAMES_PATH + "/" + this->plid);
    std::ofstream dst((std::string)DB_GAMES_PATH + "/" + this->plid + "/" + timestamp.str() + code + ".txt");

    if (!src.is_open() || !dst.is_open()) {
        std::perror("Error opening file(s)");
        exit(1);
    }

    std::string line;
    std::string firstLine;
    if (std::getline(src, firstLine)) {
        dst << firstLine << std::endl;
    }
    while (std::getline(src, line)) {
        dst << line << std::endl;
    }

    std::istringstream iss(firstLine);
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) { args.push_back(arg); }

    timestamp.str("");
    timestamp.clear();
    if (code == "T") {
        time_t timeout = std::stoi(args[3]) + std::stoi(args[6]);
        timestamp << std::put_time(std::localtime(&timeout), "%d-%m-%Y %H:%M:%S");
        dst << timestamp.str() + " " + std::to_string(timeout) << std::endl;
    }
    else {
        timestamp << std::put_time(std::localtime(&now), "%d-%m-%Y %H:%M:%S");
        dst << timestamp.str() + " " + std::to_string(now - init) << std::endl;
    }

    src.close();
    dst.close();

    if (std::remove(srcFileName.c_str()) == ERROR) {
        std::perror("Error deleting file");
        exit(1);
    }
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