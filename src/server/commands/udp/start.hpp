#ifndef START_COMMAND_H
#define START_COMMAND_H

#include "../command.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
#include <random>
#include <ctime>

class StartCommand : public UDPCommand {

    private:
        std::string max_playtime;

    public:
        StartCommand(std::string plid, std::string max_playtime) 
            : UDPCommand(START, START_RESP, plid), max_playtime(max_playtime) {};

        bool check() override;
        std::string exec() override;
        std::string genColors();
};

#endif