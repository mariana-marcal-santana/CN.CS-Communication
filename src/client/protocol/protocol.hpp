#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <string>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <iostream>
#include <memory>
#include <cstring>

#include "../constants.hpp"

class Client {
    public:
        Client(std::string plid, std::string serverIP, std::string serverPort)
            : plid(plid), tries(1), serverIP(serverIP), serverPort(serverPort) {};
        
        std::string plid;
        int tries;
        int setupConnection();
        int sockfd;
        struct addrinfo hints, *res;
        struct sockaddr_in addr;

        ~Client();

    protected:
        std::string serverIP;
        std::string serverPort;
};

#endif