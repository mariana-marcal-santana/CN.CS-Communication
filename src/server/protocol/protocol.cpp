#include "protocol.hpp"

Server::~Server() {
    if (this->udp_sockfd >= 0) {
        close(this->udp_sockfd);
    }
    if (this->tcp_sockfd >= 0) {
        close(this->tcp_sockfd);
    }
}

int Server::setupConnection() {

    this->udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->udp_sockfd == ERROR) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&udp_hints, 0, sizeof(udp_hints));
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(clientIP.c_str(), clientPort.c_str(), &this->udp_hints, &this->udp_res) != 0) {
        perror("Error getting address info");
        exit(1);
    }
    return 0;
}