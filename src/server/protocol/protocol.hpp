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

class Server {
    public:
        Server(std::string plid, std::string serverIP, std::string serverPort)
            : plid(plid), tries(1), clientIP(serverIP), clientPort(serverPort) {};
        
        std::string plid;
        int tries;
        std::string clientIP;
        std::string clientPort;
        int setupConnection();
        int udp_sockfd, tcp_sockfd;
        struct addrinfo udp_hints, *udp_res, tcp_hints, *tcp_res;
        struct sockaddr_in addr;

        ~Server();        
};

#endif