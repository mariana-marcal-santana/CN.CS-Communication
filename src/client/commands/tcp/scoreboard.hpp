#ifndef SCOREBOARD_COMMAND_HPP
#define SCOREBOARD_COMMAND_HPP

#include "command.hpp"

class ScoreboardCommand : public Command {

    private:

    public:
        ScoreboardCommand() 
            : Command(TCP, START) {};

        void send() override;
        void receive() override;
        int execute() override;
        std::string formatData() override;
};

#endif