#include "protocol.hpp"

Client::~Client() {
    if (this->sockfd >= 0) {
        close(this->sockfd);
    }
}

int Client::setupConnection() {

    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->sockfd == ERROR) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo(serverIP.c_str(), serverPort.c_str(), &this->hints, &this->res) != 0) {
        perror("Error getting address info");
        exit(1);
    }
    return 0;
}