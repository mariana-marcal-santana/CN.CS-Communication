#ifndef TRY_COMMAND_H
#define TRY_COMMAND_H

#include "command.hpp"

class TryCommand : public Command {

    private:
        std::string C1;
        std::string C2;
        std::string C3;
        std::string C4;

    public:
        TryCommand(std::string C1, std::string C2, std::string C3, std::string C4) 
            : Command(UDP, START), C1(C1), C2(C2), C3(C3), C4(C4) {};

        void send() override;
        void receive() override;
        int execute() override;
        std::string formatData() override;
};

#endif