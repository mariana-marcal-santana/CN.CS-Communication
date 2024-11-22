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
    static Command* createCommand(const std::string &input);

private:
    // UDP commands
    static Command* commandStart(std::vector<std::string> args);
    static Command* commandTry(std::vector<std::string> args);
    static Command* commandExit(std::vector<std::string> args);
    static Command* commandQuit(std::vector<std::string> args);
    static Command* commandDebug(std::vector<std::string> args);

    // TCP commands
    static Command* commandShowTrials(std::vector<std::string> args);
    static Command* commandScoreboard(std::vector<std::string> args);
};

#endif // CLIENT_COMMAND_HANLDER_H