#ifndef TRY_COMMAND_H
#define TRY_COMMAND_H

#include "command.hpp"
#include <iostream>
#include <sstream>
#include <vector>

class TryCommand : public Command {

    private:
        std::string C1;
        std::string C2;
        std::string C3;
        std::string C4;

    public:
        TryCommand(std::string C1, std::string C2, std::string C3, std::string C4) 
            : Command(UDP, START), C1(C1), C2(C2), C3(C3), C4(C4) {};

        void receive() override;
        std::string formatData() override;
};

#endif