#ifndef DEBUG_COMMAND_H
#define DEBUG_COMMAND_H

#include "../command.hpp"

class DebugCommand : public Command {

    private:
        std::string plid;
        std::string max_playtime;
        std::string C1;
        std::string C2;
        std::string C3;
        std::string C4;

    public:
        DebugCommand() 
            : Command(UDP, START), plid(plid), max_playtime(max_playtime), C1(C1), C2(C2), C3(C3), C4(C4) {};

        void send() override;
        void receive() override;
        int execute() override;
        std::string formatData() override;
};

#endif