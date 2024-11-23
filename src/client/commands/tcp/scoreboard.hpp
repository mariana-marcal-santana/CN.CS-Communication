#ifndef SCOREBOARD_COMMAND_HPP
#define SCOREBOARD_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>

class ScoreboardCommand : public Command {
    public:
        ScoreboardCommand() 
            : Command(TCP, START) {};

        void receive() override;
        std::string formatData() override;
};

#endif