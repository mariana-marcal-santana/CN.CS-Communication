#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <vector>
#include <istream>
#include <algorithm>
#include <sstream>
#include "command.hpp"
#include "udp/start.hpp"
#include "udp/try.hpp"
#include "udp/quit_exit.hpp"
#include "udp/debug.hpp"
#include "tcp/showtrials.hpp"
#include "tcp/scoreboard.hpp"

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