#ifndef SHOWTRIALS_COMMAND_HPP
#define SHOWTRIALS_COMMAND_HPP

#include "../command.hpp"
#include <sstream>
#include <vector>

class ShowTrialsCommand : public TCPCommand {
    public:
        std::string plid;

        ShowTrialsCommand(std::string plid) 
            : TCPCommand(SHOW_TRIALS, SHOW_TRIALS_RESP), plid(plid) {};

        bool check() override;
        std::string exec() override;
};

#endif