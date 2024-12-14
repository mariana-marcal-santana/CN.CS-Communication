#ifndef QUIT_EXIT_COMMAND_H
#define QUIT_EXIT_COMMAND_H

#include "../command.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>


class QuitExitCommand : public UDPCommand {
    private:
        std::string plid;

    public:
        QuitExitCommand(const std::string& plid)
        : UDPCommand(QUIT_EXIT, QUIT_EXIT_RESP), plid(plid) {};

        bool check() override;
        std::string exec() override;
};

#endif