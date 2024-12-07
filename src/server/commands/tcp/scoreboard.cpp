#include "scoreboard.hpp"

bool ScoreboardCommand::check() {
    return true;
}

std::string ScoreboardCommand::exec() {
    std::ifstream file("scoreboard.txt");
    if (!file.is_open()) {
        perror("Unable to open file");
        exit(1);
    }

    // no won games
    if (std::filesystem::directory_iterator(DB_SCORES_PATH) == std::filesystem::directory_iterator{}) {
        return "RSS EMPTY\n";
    }

    std::vector<std::filesystem::directory_entry> files;
    for (const auto& entry : std::filesystem::directory_iterator(DB_SCORES_PATH)) {
        if (entry.is_regular_file()) {
            files.push_back(entry);
        }
    }
    std::sort(files.begin(), files.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
        return a.path().filename() < b.path().filename();
    });


    std::string result = "RSS OK ";

    std::string line;
    std::getline(file, line);
    file.close();
    return "RSC " + line + "\n";
}