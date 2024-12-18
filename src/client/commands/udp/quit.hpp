#ifndef QUIT_COMMAND_H
#define QUIT_COMMAND_H

#include "../command.hpp"
#include <sstream>
#include <vector>

class QuitCommand : public UDPCommand {

    public:
        QuitCommand() 
            : UDPCommand(QUIT) {};

        bool shouldSend() override;
        void handleReceive() override;
        std::string formatData() override;
};

#endif