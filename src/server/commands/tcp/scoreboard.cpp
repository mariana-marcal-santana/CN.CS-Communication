#include "scoreboard.hpp"

bool ScoreboardCommand::check() {
    return true;
}

std::string ScoreboardCommand::formatScoreboardLine(int i, std::filesystem::path path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        perror("Unable to open file");
        exit(1);
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::istringstream iss(line);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) { args.push_back(arg); }

    std::string result = "             " + std::to_string(i) + " -  ";
    result += args[0] + "  " + args[1] + "     " + args[2] + "        " + args[3] + "       ";
    result += args[4] == "P" ? "PLAY" : "DEBUG";

    return result;
}

std::string ScoreboardCommand::exec() {

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

    std::ofstream scoreboard("scoreboard.txt");
    if (!scoreboard.is_open()) {
        perror("Unable to open file");
        exit(1);
    }

    scoreboard << "-------------------------------- TOP 10 SCORES --------------------------------\n\n";
    scoreboard << "                 SCORE PLAYER     CODE    NO TRIALS   MODE\n\n";

    for (int i = 0; i < 10 && i < static_cast<int>(files.size()); i++)
        scoreboard << this->formatScoreboardLine(i + 1, files[i].path()) << std::endl;

    std::ostringstream content;
    content << scoreboard.rdbuf();

    scoreboard.close();

    auto size = std::filesystem::file_size("scoreboard.txt");

    return "RSS OK scoreboard.txt " + std::to_string(size) + " " + content.str();
}