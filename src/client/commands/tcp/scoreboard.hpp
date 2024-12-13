#ifndef SCOREBOARD_COMMAND_HPP
#define SCOREBOARD_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>
#include <fstream> 

class ScoreboardCommand : public TCPCommand {
    public:
        ScoreboardCommand() 
            : TCPCommand(SCOREBOARD) {};

        void receive() override;
        void handleReceive() override;
        std::string formatData() override;
};

#endif