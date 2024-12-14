#ifndef START_COMMAND_H
#define START_COMMAND_H

#include "../command.hpp"
#include <iostream>
#include <sstream>
#include <vector>

class StartCommand : public UDPCommand {

    private:
        std::string plid;
        std::string max_playtime;

    public:
        StartCommand(std::string plid, std::string max_playtime) 
            : UDPCommand(START), plid(plid), max_playtime(max_playtime) {};

        void handleReceive() override;
        std::string formatData() override;
};

#endif