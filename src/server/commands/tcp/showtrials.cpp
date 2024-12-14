#include "showtrials.hpp"

bool ShowTrialsCommand::check() {
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    return true;
}

std::string ShowTrialsCommand::exec() {

    std::string result;
    std::string fileName;

    std::string playerInfo = this->findPlayerInfo(this->plid);
    if (playerInfo != "") {
        fileName = (std::string)DB_GAMES_PATH + "/GAME_" + this->plid + ".txt";
        result = "RST ACT ";
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
            fileName = files[0].path().filename();
        }
        else 
            return "RST NOK\n";
    }

    std::ifstream file(fileName);
    if (!file.is_open()) {
        perror("Unable to open file");
        exit(1);
    }


    std::string line;
    std::getline(file, line);
    file.close();
    return "RST ACT " + line + "\n";
}