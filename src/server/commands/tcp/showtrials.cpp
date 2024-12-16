#include "showtrials.hpp"
int FindLastGame(const std::string& PLID, std::string& fname) {
    namespace fs = std::filesystem;

    std::string dirname = "GAMES/" + PLID + "/";
    fs::path dirPath(dirname);

    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        return 0;
    }

    fs::path lastFile;
    bool found = false;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file() && entry.path().filename().string()[0] != '.') {
            lastFile = entry.path();
            found = true;
        }
    }

    if (found) {
        fname = lastFile.string();
    }

    return found ? 1 : 0;
}


bool ShowTrialsCommand::check() {
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    return true;
}

std::string ShowTrialsCommand::exec() {
    std::string playerInfo = this->findPlayerInfo(this->plid);
    std::string response;

    namespace fs = std::filesystem;
    std::string fileName;
    fs::path filePath;
    std::string status;

    if (playerInfo != ""){
        fileName = "GAME_" + this->plid + ".txt";   
        filePath = (fs::path)(DB_GAMES_PATH + '/' + fileName);
        status = ACT;
    }
    else {
        if(FindLastGame(this->plid, fileName) == 0)
            return SHOW_TRIALS_RESP + ' ' + (std::string)NOK;

        filePath = (fs::path)(DB_GAMES_PATH + '/' + this->plid + '/' + fileName);
        status = FIN;
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        perror("Unable to open file");
        exit(1);
    }

    auto size = std::filesystem::file_size(filePath);
    response = SHOW_TRIALS_RESP + ' ' + status + ' ' + fileName + ' ' + std::to_string(size) + ' ';
    
    std::string line;
    std::getline(file, line);
    while(std::getline(file, line)){
        response += line;
    }
    file.close();

    //na ultima linha aparece o tempo que ainda têm para jogar
    std::istringstream iss(playerInfo);
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
    }

    std::time_t sec = std::time(nullptr);
    response += (std::to_string)(std::stoi(args[3]) - (std::stoi(args[6]) - sec));
    return response + "\n";
}

