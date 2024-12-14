#include "quit_exit.hpp"

bool QuitExitCommand::check() {
    if (this->plid.length() != 6 || !std::all_of(this->plid.begin(), this->plid.end(), ::isdigit)) {
        return false;
    }
    return true;
}

std::string QuitExitCommand::exec() {

    // playerInfo: plid mode colors time date hour timestamp
    std::string playerInfo = this->findPlayerInfo(this->plid);
    
    // no active game
    if (playerInfo == "")
        return "RQT NOK\n";

    std::istringstream iss(playerInfo);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    std::time_t now = std::time(nullptr);
    this->logGame("Q", now, std::stoi(args[6]));
    
    std::string result = "RQT OK";
    for (size_t i = 0; i < args[2].length(); i++) {
        result += " " + args[2].substr(i, 1);
    }

    return result + " \n";
}