#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include <string>
#include <memory>
#include "../protocol/protocol.hpp"

class Command
{
    protected:
        int networkType;
        std::string command;
        std::string data;

    public:
        std::unique_ptr<Client> client;

        Command(std::string command) 
            : command(command) {}

        Command(int networkType, std::string command) 
            : networkType(networkType), command(command) {}

        virtual ~Command() = default;

        virtual void send() = 0;
        virtual int execute() = 0;
        virtual void receive() = 0;
        virtual void handleReceive() = 0;
        virtual std::string formatData() = 0;
};

class UDPCommand : public Command
{
    public:
        UDPCommand(std::string command) 
            : Command(UDP, command) {}

    void send() override;
    int execute() override;
    void receive() override;
    virtual void handleReceive() = 0;
    virtual std::string formatData() = 0;
};

class TCPCommand : public Command
{
    public:
        TCPCommand(std::string command) 
            : Command(TCP, command) {}

    void send() override;
    int execute() override;
    void receive() override;
    virtual void handleReceive() = 0;
    virtual std::string formatData() = 0;
};

// {
//     protected:
//         std::unique_ptr<Client> client;
//         int networkType;
//         std::string command;        

//     public:

//         Command(int networkType, std::string command) 
//             : networkType(networkType), command(command) {}

//         virtual ~Command() = default;

//         void send();
//         int execute();
//         virtual void receive() = 0;
//         virtual std::string formatData() = 0;

//         void setClient(std::string serverIp, std::string serverPort);
// };

#endif // CLIENT_COMMAND_H