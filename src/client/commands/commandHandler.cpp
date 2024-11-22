#include "commandHandler.hpp"
#include "commands/udp/start.hpp"

Command* CommandHandler::createCommand(const std::string &input) {

    // parse input
    std::istringstream iss(input);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    // create specific command
    if (args[0] == START) {
        return CommandHandler::commandStart(args);
    } 
    else if (args[0] == TRY) {
        return CommandHandler::commandTry(args);
    } 
    else if (args[0] == SHOW_TRIALS || args[0] == ST) {
        return CommandHandler::commandShowTrials(args);
    }
    else if (args[0] == SCOREBOARD || args[0] == SB) {
        return CommandHandler::commandScoreboard(args);
    }
    else if (args[0] == QUIT) {
        return CommandHandler::commandQuit(args);
    }
    else if (args[0] == EXIT) {
        return CommandHandler::commandExit(args);
    }
    else if (args[0] == DEBUG) {
        return CommandHandler::commandDebug(args);
    }
    else { return nullptr; }
}

Command* CommandHandler::commandStart(std::vector<std::string> args) {

    if (args.size() != 3) {
        printf("%s: start  PLID  max_playtime\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    // verificar validade do PLID
    if (args[1].size() != 6) {
        printf("%s: PLID must be a 6-digit IST student number\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    if (std::stoi(args[2]) > 600) {
        printf("%s: max_playtime must be less than 600\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    return new StartCommand(args[1], args[2]);
}

Command* CommandHandler::commandTry(std::vector<std::string> args) {

    if (args.size() != 5) {
        printf("%s: try C1 C2 C3 C4\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    for (int i = 1; i < 5; i++) {
        if (std::find(colors.begin(), colors.end(), args[1]) == colors.end()) {
            printf("%s: invalid color names\n", INVALID_COMMAND_MSG);
            return nullptr;
        }
    }

    return new TryCommand(args[1], args[2], args[3], args[4]);
}

Command* CommandHandler::commandShowTrials(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: show_trials\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    return new ShowTrialsCommand();
}

Command* CommandHandler::commandScoreboard(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: scoreboard\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    return new ScoreboardCommand();
}

Command* CommandHandler::commandQuit(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: quit\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    return new QuitCommand();
}

Command* CommandHandler::commandExit(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: exit\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    return new ExitCommand();
}

Command* CommandHandler::commandDebug(std::vector<std::string> args) {

    if (args.size() != 7) {
        printf("%s: debug PLID max_playtime C1 C2 C3 C4\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    // verificar validade do PLID

    if (std::stoi(args[2]) > 600) {
        printf("%s: max_playtime must be less than 600\n", INVALID_COMMAND_MSG);
        return nullptr;
    }

    std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    for (int i = 3; i < 7; i++) {
        if (std::find(colors.begin(), colors.end(), args[1]) == colors.end()) {
            printf("%s: invalid color names\n", INVALID_COMMAND_MSG);
            return nullptr;
        }
    }

    return new DebugCommand(args[1], args[2], args[3], args[4], args[5], args[6]);
}