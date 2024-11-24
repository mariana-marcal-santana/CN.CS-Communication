#ifndef SCOREBOARD_COMMAND_HPP
#define SCOREBOARD_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>

class ScoreboardCommand : public TCPCommand {
    public:
        ScoreboardCommand() 
            : TCPCommand(SCOREBOARD) {};

        void handleReceive() override;
        std::string formatData() override;
};

#endif