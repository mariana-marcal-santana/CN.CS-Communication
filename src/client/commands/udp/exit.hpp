#ifndef EXIT_COMMAND_H
#define EXIT_COMMAND_H

#include "../command.hpp"

class ExitCommand : public Command {

    private:
        std::string plid;
        std::string max_playtime;

    public:
        ExitCommand() 
            : Command(UDP, START) {};

        void send() override;
        void receive() override;
        int execute() override;
        std::string formatData() override;
};

#endif