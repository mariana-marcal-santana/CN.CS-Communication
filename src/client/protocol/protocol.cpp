#include "protocol.hpp"

Client::~Client() {}

int Client::setupConnection() {

    this->udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->udp_sockfd == ERROR) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&udp_hints, 0, sizeof(udp_hints));
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(serverIP.c_str(), serverPort.c_str(), &this->udp_hints, &this->udp_res) == ERROR) {
        perror("Error getting address info");
        exit(1);
    }
    return 0;
}