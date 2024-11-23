#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "../command.hpp"
#include <sstream>
#include <vector>

class ExitCommand : public Command {

    public:
        ExitCommand() 
            : Command(UDP, EXIT) {};

        void receive() override;
        std::string formatData() override;
};

#endif