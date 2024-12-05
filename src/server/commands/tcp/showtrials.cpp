#include "showtrials.hpp"

bool ShowTrialsCommand::check() {
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    return true;
}

std::string ShowTrialsCommand::exec() {
    std::string fileName = "GAME_" + this->plid + ".txt";
    std::ifstream file(fileName);
    if (file.is_open() == ERROR) {
        perror("Unable to open file");
        exit(1);
    }

    
    std::string line;
    std::getline(file, line);
    file.close();
    return "RST ACT " + line + "\n";
}