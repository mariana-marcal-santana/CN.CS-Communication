#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "../command.hpp"
#include <sstream>
#include <vector>

class ExitCommand : public UDPCommand {

    public:
        ExitCommand() 
            : UDPCommand(EXIT) {};

        bool shouldSend() override;
        void handleReceive() override;
        std::string formatData() override;
};

#endif