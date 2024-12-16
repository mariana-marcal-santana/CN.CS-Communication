#include "showtrials.hpp"

bool ShowTrialsCommand::check() {
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    return true;
}

std::string ShowTrialsCommand::exec() {

    bool currentGame = false;

    std::string result;
    std::string fileName;

    std::string playerInfo = this->findPlayerInfo(this->plid);
    if (playerInfo != "") {
        fileName = (std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt";
        result = "RST ACT ";
        currentGame = true;
    }
    else {
        std::string dirPath = (std::string)DB_GAMES_PATH + "/" + this->plid;
        if (std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath)) {
            
            std::vector<std::filesystem::directory_entry> files;
            for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry);
                }
            }
            std::sort(files.begin(), files.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
                return a.path().filename() > b.path().filename();
            });
            fileName = files[0].path();
            result = "RST FIN ";
        }
        else 
            return "RST NOK\n";
    }

    std::string trialsFileName = "trials_" + this->plid + ".txt";
    std::ofstream trials(trialsFileName);
    if (!trials.is_open()) {
        perror("Unable to open file");
        exit(1);
    }

    std::string to_write = currentGame ? "Active game found for player " : "Last finalized game for player ";

    printf("File name: %s\n", fileName.c_str());
    std::ifstream file(fileName);
    if (!file.is_open()) {
        perror("Unable to open file");
        exit(1);
    }
    std::getline(file, playerInfo);
    
    printf("Player info: %s\n", playerInfo.c_str());
    fflush(stdout);
    std::istringstream iss(playerInfo);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    trials << std::endl << to_write + this->plid << std::endl;
    trials << "Game initiated: " + args[4] + " " + args[5] + " with " + args[3] + " seconds to be completed" << std::endl;
    
    if (!currentGame) {
        trials << "Mode : " << ((args[1] == "P") ? "PLAY" : "DEBUG");
        trials << "  Secret code: " + args[2] << std::endl;
        std::string secretKey = args[1] + " " + args[2] + " " + args[3] + " " + args[6];
    }

    std::vector<std::string> tries;
    while (std::getline(file, playerInfo)) {
        if (playerInfo[0] == 'T') {
            tries.push_back(playerInfo);
        }
    }
    
    trials << std::endl << "     --- Transactions found: " + std::to_string(tries.size()) + " ---" << std::endl << std::endl;

    for (std::string t : tries) {

        std::istringstream iss(t);
        std::string arg_t;
        std::vector<std::string> args_t;
        while (iss >> arg_t) { args_t.push_back(arg_t); }
        trials << "Trial: " << args_t[1] + ", nB: " + args_t[2] + ", nW: " + args_t[3] + " at " + args_t[4] + "s" << std::endl;
    }

    if (currentGame) {
        time_t now = std::time(nullptr);
        time_t time_remaining = std::stoi(args[3]) - now + std::stoi(args[6]);
        trials << std::endl << "  -- " + std::to_string(time_remaining) + " seconds remaining to be completed --" << std::endl; 
    }
    else {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            perror("Unable to open file");
            exit(1);
        }
        std::string line;
        std::string lastLine;
        while (std::getline(file, line)) {
            lastLine = line;
        }
        file.close();
        std::istringstream iss(lastLine);
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) { args.push_back(arg); }
        trials << std::endl << "  -- Game completed at " + args[0] + " " + args[1] + ", Duration: " + args[2] + "s" << std::endl;
    }

    trials.close();

    std::ifstream r_trials(trialsFileName.c_str());
    if (!r_trials.is_open()) {
        perror("Unable to open file");
        exit(1);
    }

    std::stringstream content;
    content << r_trials.rdbuf();

    r_trials.close();

    auto size = std::filesystem::file_size(trialsFileName);

    return result + trialsFileName + " " + std::to_string(size) + " " + content.str();
}
