#ifndef TRY_COMMAND_H
#define TRY_COMMAND_H

#include "../command.hpp"
#include "../../common.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

class TryCommand : public UDPCommand {

    private:
        std::string plid;
        std::string C1;
        std::string C2;
        std::string C3;
        std::string C4;
        std::string nT;
        int nB = 0;
        int nW = 0;

    public:
        TryCommand(std::string plid, std::string C1, std::string C2, std::string C3, std::string C4, std::string nT) 
            : UDPCommand(TRY), plid(plid), C1(C1), C2(C2), C3(C3), C4(C4), nT(nT) {};

        bool check() override;
        std::string exec() override;
        //std::string formatData() override;
        std::string evalTry(std::string solution);
        std::vector<std::string> getPlayerTries(std::string plid);
};

#endif