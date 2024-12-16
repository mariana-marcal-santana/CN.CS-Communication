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
        return a.path().filename() > b.path().filename();
    });

    std::string scoreboardFileName = (std::string)DB_PATH + "scoreboard.txt";

    std::ofstream w_scoreboard(scoreboardFileName.c_str());
    if (!w_scoreboard.is_open()) {
        perror("Unable to open file (write)");
        exit(1);
    }

    w_scoreboard << "-------------------------------- TOP 10 SCORES --------------------------------\n\n";
    w_scoreboard << "                 SCORE PLAYER     CODE    NO TRIALS   MODE\n\n";

    for (int i = 0; i < 10 && i < static_cast<int>(files.size()); i++)
        w_scoreboard << this->formatScoreboardLine(i + 1, files[i].path()) << std::endl;

    w_scoreboard.close();

    std::ifstream r_scoreboard(scoreboardFileName.c_str());
    if (!r_scoreboard.is_open()) {
        perror("Unable to open file (read)");
        exit(1);
    }

    std::stringstream content;
    content << r_scoreboard.rdbuf();

    r_scoreboard.close();

    auto size = std::filesystem::file_size(scoreboardFileName);

    if (std::remove(scoreboardFileName.c_str()) == ERROR) {
        std::perror("Error deleting file");
    }

    return "RSS OK " + scoreboardFileName + std::to_string(size) + " " + content.str() + "\n";
}