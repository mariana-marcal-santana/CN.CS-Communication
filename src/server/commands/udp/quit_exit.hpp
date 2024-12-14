#ifndef QUIT_EXIT_COMMAND_H
#define QUIT_EXIT_COMMAND_H

#include "../command.hpp"
#include <sstream>
#include <vector>

class QuitExitCommand : public UDPCommand {

    public:
        QuitExitCommand(std::string plid) 
            : UDPCommand(QUIT_EXIT, QUIT_EXIT_RESP, plid) {};

    bool check() override;
    std::string exec() override;
};

#endif