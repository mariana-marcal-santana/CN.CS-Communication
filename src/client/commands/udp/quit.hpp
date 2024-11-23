#ifndef QUIT_COMMAND_H
#define QUIT_COMMAND_H

#include "../command.hpp"
#include <sstream>
#include <vector>

class QuitCommand : public Command {

    public:
        QuitCommand() 
            : Command(UDP, QUIT) {};

        void receive() override;
        std::string formatData() override;
};

#endif