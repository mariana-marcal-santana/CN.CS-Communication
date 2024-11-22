#ifndef SHOWTRIALS_COMMAND_HPP
#define SHOWTRIALS_COMMAND_HPP

#include "command.hpp"

class ShowTrialsCommand : public Command {

    private:

    public:
        ShowTrialsCommand() 
            : Command(TCP, START) {};

        void send() override;
        void receive() override;
        int execute() override;
        std::string formatData() override;
};

#endif