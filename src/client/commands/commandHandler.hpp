#ifndef CLIENT_COMMAND_HANLDER_H
#define CLIENT_COMMAND_HANLDER_H

#include "../constants.hpp"

#include "command.hpp"
#include "udp/start.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

class CommandHandler {
public:
    static Command* createCommand(std::vector<std::string> args);

private:
    // UDP commands
    static bool verifCommandStart(std::vector<std::string> args);
    static bool verifCommandTry(std::vector<std::string> args);
    static bool verifCommandExit(std::vector<std::string> args);
    static bool verifCommandQuit(std::vector<std::string> args);
    static bool verifCommandDebug(std::vector<std::string> args);

    // TCP commands
    static bool verifCommandShowTrials(std::vector<std::string> args);
    static bool verifCommandScoreboard(std::vector<std::string> args);
};

#endif // CLIENT_COMMAND_HANLDER_H