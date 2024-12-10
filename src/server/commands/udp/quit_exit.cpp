#include "quit_exit.hpp"


bool QuitExitCommand::check() {
    return this->plid.length() == 6 && std::all_of(this->plid.begin(), this->plid.end(), ::isdigit);
}

std::string QuitExitCommand::exec(){
    
    std::string playerInfo = findPlayerInfo(this->plid);
    if (playerInfo == "")
        return "RQT NOK\n";

    namespace fs = std::filesystem;
    fs::path dirPath = fs::path(DB_GAMES_PATH + '/' + this->plid);
    fs::path filePath = fs::path((std::string)DB_GAMES_PATH + '/' + "GAMES_" + this->plid + ".txt");

    if (!fs::exists(dirPath)) {
        fs::create_directory(dirPath);
    }

    std::istringstream iss(playerInfo);
    std::string str;
    std::vector<std::string> args;
    while (iss >> str) { args.push_back(str); }

    std::string date = args[4] + "_" + args[5];

    date.erase(std::remove_if(date.begin(), date.end(), [](char c) {
        return c == '-' || c == ':';
    }), date.end());

    std::string newfileName = date + "_" + "X" + ".txt"; //o code não está bem, falta fazer

    fs::path newfilePath = dirPath / fs::path(newfileName);

    try {
        fs::rename(filePath, newfilePath);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error renaming file: " << e.what() << "\n";
    }
    
    return (std::string)"RQT OK " + playerInfo[9] + " " + playerInfo[10] + " " + playerInfo[11] + " " + playerInfo[12] + "\n";
}


