#ifndef SHOWTRIALS_COMMAND_HPP
#define SHOWTRIALS_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>

class ShowTrialsCommand : public Command {
    public:
        ShowTrialsCommand() 
            : Command(TCP, START) {};

        void receive() override;
        std::string formatData() override;
};

#endif