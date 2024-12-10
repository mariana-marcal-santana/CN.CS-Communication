#ifndef SCOREBOARD_COMMAND_HPP
#define SCOREBOARD_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>

class ScoreboardCommand : public TCPCommand {
    public:
        ScoreboardCommand() 
            : TCPCommand(SCOREBOARD) {};

        bool check() override;
        std::string exec() override;
};

#endif