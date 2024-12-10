#ifndef START_COMMAND_H
#define START_COMMAND_H

#include "../command.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class StartCommand : public UDPCommand {
    private:
        std::string plid;
        int max_playtime;

        std::string generateColorKey();

    public:
        StartCommand(const std::string& plid, const std::string& timeStr)
        : UDPCommand(START), plid(plid), max_playtime(std::stoi(timeStr)) {};

        bool check() override;
        std::string exec() override;
};

#endif