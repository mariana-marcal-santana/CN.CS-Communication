#include "scoreboard.hpp"

bool ScoreboardCommand::check() {
    return true;
}

std::string ScoreboardCommand::exec() {
    std::ifstream file("scoreboard.txt");
    if (file.is_open() == ERROR) {
        perror("Unable to open file");
        exit(1);
    }

    // no won games
    if (std::filesystem::exists(DB_SCORES_PATH) && std::filesystem::is_directory(DB_SCORES_PATH)) {
        if (std::filesystem::directory_iterator(DB_SCORES_PATH) == std::filesystem::directory_iterator{}) {
            return "RSS EMPTY\n";
        }
    }

    std::string result = "RSS OK ";

    std::string line;
    std::getline(file, line);
    file.close();
    return "RSC " + line + "\n";
}