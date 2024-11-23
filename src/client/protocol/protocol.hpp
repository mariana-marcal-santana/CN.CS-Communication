#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>

#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <iostream>
#include <exception>

#include "../constants.hpp"

class Client {
    public:
        ~Client();
        virtual int sendData(const std::string& data) = 0;
        virtual std::string receiveData() = 0;
        int plid;
        int tries;
        bool hasStartedGame();

    protected:
        int sockfd = -1;
        std::string serverIP;
        int serverPort;
        struct sockaddr_in serverAddr;        
};

class UDPClient : public Client {
    public:
        UDPClient(std::string serverIP, int serverPort);
        int sendData(const std::string& data) override;
        std::string receiveData();

    private:
        int type = UDP;
        //bool verbose = TCP_VERBOSE;
};

class TCPClient : public Client {
    public:
        TCPClient(std::string serverIP, int serverPort);
        int sendData(const std::string& data) override;
        std::string receiveData();

    private:
        int type = TCP;
        //bool verbose = TCP_VERBOSE;
};



#endif