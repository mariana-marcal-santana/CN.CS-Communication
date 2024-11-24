#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "../command.hpp"
#include <sstream>
#include <vector>

class ExitCommand : public UDPCommand {

    public:
        ExitCommand() 
            : UDPCommand(EXIT) {};

        void handleReceive() override;
        std::string formatData() override;
};

#endif