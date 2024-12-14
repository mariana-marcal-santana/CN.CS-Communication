#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <ctime>
#include <iomanip>


#include "../constants.hpp"

class Command
{
    protected:
        int networkType;
        std::string command;
        std::string cmd_response;
        
    public:

        Command(int networkType, std::string command, std::string cmd_response) 
            : networkType(networkType), command(command), cmd_response(cmd_response) {}

        virtual ~Command() = default;

        std::string data;
        virtual std::string execute() = 0;
        virtual bool check() = 0;
        virtual std::string exec() = 0;
        std::string findPlayerInfo(std::string plid);
        std::vector<std::string> getPlayerTries(std::string plid);
        int createPlayerFile(std::string plid, char mode, std::string key, int time);
};

class UDPCommand : public Command
{
    public:
        UDPCommand(std::string command, std::string cmd_response) 
            : Command(UDP, command, cmd_response) {}

    std::string execute() override;
    virtual bool check() = 0;
    virtual std::string exec() = 0; 
};

class TCPCommand : public Command
{
    public:
        TCPCommand(std::string command, std::string cmd_response) 
            : Command(TCP, command, cmd_response) {}

    std::string execute() override;
    virtual bool check() = 0;
    virtual std::string exec() = 0;
};


#endif