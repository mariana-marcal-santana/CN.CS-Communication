#include "commandHandler.hpp"
#include "udp/start.hpp"
#include "udp/try.hpp"
#include "tcp/showtrials.hpp"
#include "tcp/scoreboard.hpp"
#include "udp/exit.hpp"
#include "udp/quit.hpp"
#include "udp/debug.hpp"

Command* CommandHandler::createCommand(std::vector<std::string> args) {

    // create specific command
    if (args[0] == START) {
        if (CommandHandler::verifCommandStart(args)) {
            printf("start\n");
            return new StartCommand(args[1], args[2]);
        }
    } 
    else if (args[0] == TRY) {
        if (CommandHandler::verifCommandTry(args)) {
            return new TryCommand(args[1], args[2], args[3], args[4]);
        }
    } 
    else if (args[0] == SHOW_TRIALS || args[0] == ST) {
        if (CommandHandler::verifCommandShowTrials(args)) {
            return new ShowTrialsCommand();
        }
    } 
    else if (args[0] == SCOREBOARD || args[0] == SB) {
        if (CommandHandler::verifCommandScoreboard(args)) {
            return new ScoreboardCommand();
        }
    } 
    else if (args[0] == QUIT) {
        if (CommandHandler::verifCommandQuit(args)) {
            return new QuitCommand();
        }
    } 
    else if (args[0] == EXIT) {
        if (CommandHandler::verifCommandExit(args)) {
            return new ExitCommand();
        }
    } 
    else if (args[0] == DEBUG) {
        if (CommandHandler::verifCommandDebug(args)) {
            return new DebugCommand(args[1], args[2], args[3], args[4], args[5], args[6]);
        }
    } 
    return nullptr;
}

bool CommandHandler::verifCommandStart(std::vector<std::string> args) {

    if (args.size() != 3) {
        printf("%s: start  PLID  max_playtime\n", INVALID_COMMAND_MSG);
        return false;
    }

    if (args[1].size() != 6) {
        printf("%s: PLID must be a 6-digit IST student number\n", INVALID_COMMAND_MSG);
        return false;
    }

    if (std::stoi(args[2]) > 600) {
        printf("%s: max_playtime must be less than 600\n", INVALID_COMMAND_MSG);
        return false;
    }

    return true;
}

bool CommandHandler::verifCommandTry(std::vector<std::string> args) {

    if (args.size() != 5) {
        printf("%s: try C1 C2 C3 C4\n", INVALID_COMMAND_MSG);
        return false;
    }

    std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    for (int i = 1; i < 5; i++) {
        if (std::find(colors.begin(), colors.end(), args[1]) == colors.end()) {
            printf("%s: invalid color names\n", INVALID_COMMAND_MSG);
            return false;
        }
    }

    return true;
}

bool CommandHandler::verifCommandShowTrials(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: show_trials\n", INVALID_COMMAND_MSG);
        return false;
    }

    return true;
}

bool CommandHandler::verifCommandScoreboard(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: scoreboard\n", INVALID_COMMAND_MSG);
        return false;
    }

    return true;
}

bool CommandHandler::verifCommandQuit(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: quit\n", INVALID_COMMAND_MSG);
        return false;
    }

    return true;
}

bool CommandHandler::verifCommandExit(std::vector<std::string> args) {

    if (args.size() != 1) {
        printf("%s: exit\n", INVALID_COMMAND_MSG);
        return false;
    }

    return true;
}

bool CommandHandler::verifCommandDebug(std::vector<std::string> args) {

    if (args.size() != 7) {
        printf("%s: debug PLID max_playtime C1 C2 C3 C4\n", INVALID_COMMAND_MSG);
        return false;
    }

    // verificar validade do PLID

    if (std::stoi(args[2]) > 600) {
        printf("%s: max_playtime must be less than 600\n", INVALID_COMMAND_MSG);
        return false;
    }

    std::vector<std::string> colors = {"R", "G", "B", "Y", "O", "P"};
    for (int i = 3; i < 7; i++) {
        if (std::find(colors.begin(), colors.end(), args[1]) == colors.end()) {
            printf("%s: invalid color names\n", INVALID_COMMAND_MSG);
            return false;
        }
    }

    return true;
}