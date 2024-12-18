#ifndef SHOWTRIALS_COMMAND_HPP
#define SHOWTRIALS_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>
#include <fstream> 

class ShowTrialsCommand : public TCPCommand {
    public:
        ShowTrialsCommand() 
            : TCPCommand(SHOW_TRIALS) {};

        bool shouldSend() override;
        void receive() override;
        void handleReceive() override;
        std::string formatData() override;
};

#endif