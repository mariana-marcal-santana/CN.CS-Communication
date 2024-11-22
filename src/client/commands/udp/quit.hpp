#ifndef QUIT_COMMAND_H
#define QUIT_COMMAND_H

#include "../command.hpp"

class QuitCommand : public Command {

    private:
        std::string plid;
        std::string max_playtime;

    public:
        QuitCommand() 
            : Command(UDP, START) {};

        void send() override;
        void receive() override;
        int execute() override;
        std::string formatData() override;
};

#endif