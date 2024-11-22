#ifndef START_COMMAND_H
#define START_COMMAND_H

#include "command.hpp"

class StartCommand : public Command {

    private:
        std::string plid;
        std::string max_playtime;

    public:
        StartCommand(std::string plid, std::string max_playtime) 
            : Command(UDP, START), plid(plid), max_playtime(max_playtime) {};

        void send() override;
        void receive() override;
        int execute() override;
        std::string formatData() override;
};

#endif