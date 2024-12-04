#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <vector>
#include <istream>
#include <algorithm>
#include <sstream>
#include "command.hpp"
//#include "server/commands/udp/start.hpp"
#include "server/commands/udp/try.hpp"
// #include "server/commands/udp/quit_exit.hpp"
// #include "server/commands/udp/debug.hpp"
// #include "server/commands/tcp/showtrials.hpp"
// #include "server/commands/tcp/scoreboard.hpp"

class CommandHandler {
    public:
        static Command* createCommand(std::string args);

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

#endif