#ifndef SHOWTRIALS_COMMAND_HPP
#define SHOWTRIALS_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>

class ShowTrialsCommand : public TCPCommand {
    public:
        ShowTrialsCommand() 
            : TCPCommand(SHOW_TRIALS) {};

        void handleReceive() override;
        std::string formatData() override;
};

#endif