#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include <string>
#include <memory>
#include "../protocol/protocol.hpp"

class Command
{
    protected:
        //ClientState* clientState;
        std::unique_ptr<Client> Client;
        std::string command;
        int networkType;

    public:
        Command(std::string command) 
            : command(command) {}

        Command(int networkType, std::string command) 
            : networkType(networkType), command(command) {}

        virtual ~Command() = default;

        virtual void send() = 0;
        virtual void receive() = 0;
        virtual int execute();
        virtual std::string formatData() = 0;

        //void setClientState(ClientState* clientState);

        void setNetworkClient(std::string serverIp, int serverPort);
};

#endif // CLIENT_COMMAND_H