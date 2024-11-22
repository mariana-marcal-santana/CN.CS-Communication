#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include <string>

#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <unistd.h>

#include "../constants.hpp"

class NetworkClient {
    public:
        ~NetworkClient();
        virtual int sendData(const std::string& data) = 0;
        virtual std::string receiveData() = 0;

    protected:
        int sockfd = -1;
        std::string serverIP;
        int serverPort;

        struct sockaddr_in serverAddr;
};

class UDPClient : public NetworkClient {
    public:
        UDPClient(std::string serverIP, int serverPort);
        ~UDPClient();
        int sendData(const std::string& data) override;
        std::string receiveData() override;
};

class UDPClient : public NetworkClient {
    public:
        UDPClient(std::string serverIP, int serverPort);
        int sendData(const std::string& data) override;
        std::string receiveData();

    private:
        int type = UDP;
        //bool verbose = TCP_VERBOSE;
};

class TCPClient : public NetworkClient {
    public:
        TCPClient(std::string serverIP, int serverPort);
        int sendData(const std::string& data) override;
        std::string receiveData();

    private:
        int type = TCP;
        //bool verbose = TCP_VERBOSE;
};



#endif