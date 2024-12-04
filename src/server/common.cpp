#include "common.hpp"

bool findPlayer(std::string plid) {
    std::ifstream file("players.txt");
    if (file.is_open() == ERROR) {
        std::cerr << "Unable to open file." << std::endl;
        exit(1);
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line == plid) {
            return true;
        }
    }
    file.close();
    return false;
}