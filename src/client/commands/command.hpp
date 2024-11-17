#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include <string>

class Command
{
    protected:
        std::string command;
    public:
        Command(std::string command) 
            : command(command) {}

        ~Command();

        // sends data to Auction Server
        virtual void send() = 0;

        // receives data from Auction Server
        virtual void receive() = 0;

        virtual int execute();
};

#endif // CLIENT_COMMAND_H