#include "command.hpp"

std::string UDPCommand::execute() {
    printf("execute");
    if (!this->check())
        return this->command + " ERR\n";
    return this->exec();
}

std::string TCPCommand::execute() {
    if (!this->check())
        return this->command + " ERR\n";
    this->data = this->exec();
    return "";
}

std::vector<std::string> TCPCommand::parseToSend() {

    // returns a vector with [id_status, fname, fsize, fdata]

    // no file to send
    if (std::count(this->data.begin(), this->data.end(), ' ') == 1) {
        
        std::string id_status = this->data.substr(0, this->data.size() - 1);
        id_status.resize(9, ' ');
        
        return {id_status};
    }

    size_t space = this->data.find(' ');
    space = this->data.find(' ', space + 1);
    std::string id_status = this->data.substr(0, space);

    printf("id_status: %s.", id_status.c_str());

    ssize_t space2 = this->data.find(' ', space + 1);
    std::string fname = this->data.substr(space + 1, space2 - space - 1);

    printf("fname: %s.", fname.c_str());
    fflush(stdout);

    space = space2;
    space2 = this->data.find(' ', space2 + 1);
    std::string fsize = this->data.substr(space + 1, space2 - space - 1);

    printf("fsize: %s.", fsize.c_str());
    fflush(stdout);

    std::string fdata = this->data.substr(space2 + 1, this->data.size() - space2 - 1);

    printf("fdata: %s.", fdata.c_str());
    fflush(stdout);
    
    return {id_status, fname, fsize, fdata};
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