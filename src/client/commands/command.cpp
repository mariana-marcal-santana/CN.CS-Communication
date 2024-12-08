#include "command.hpp"

void UDPCommand::send() {
    std::string dataToSend = this->formatData();
    printf("Sending data: %s", dataToSend.c_str());
    if (sendto(this->client->udp_sockfd, dataToSend.c_str(), dataToSend.length(), 0,
        this->client->udp_res->ai_addr, this->client->udp_res->ai_addrlen) < 0) { 
        perror("Error sending data");
        exit(1);
    }
}

int UDPCommand::execute() {
    this->send();
    printf("sent\n");
    this->receive();
    printf("Received data: %s\n", this->data.c_str());
    this->handleReceive();
    return this->command == EXIT ? 1 : 0;
}

void UDPCommand::receive() {
    struct timeval tv;
    tv.tv_sec = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    // Set timeout for receiving data
    if (setsockopt(this->client->udp_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
        exit(1);
    }

    char buffer[128];
    socklen_t len = sizeof(this->client->addr);
    int n = recvfrom(this->client->udp_sockfd, buffer, 128, 0, (struct sockaddr*)&this->client->addr, &len);

    if (n < 0) {
        perror("Error receiving data");
        if(tv.tv_sec == CONNECTION_TIMEOUT) {
            perror("Connection timeout");
        }
        exit(1);
    }

    buffer[n - 1] = '\0';
    this->data = std::string(buffer);
}

void TCPCommand::send() {

    std::string dataToSend = this->formatData();
    printf("Sending data: %s", dataToSend.c_str());

    if (write(this->client->tcp_sockfd, dataToSend.c_str(), dataToSend.length()) == ERROR) {
        perror("Error sending data");
        exit(1);
    }
}

int TCPCommand::execute() {

    this->client->tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->client->udp_sockfd == ERROR) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&this->client->tcp_hints, 0, sizeof(this->client->tcp_hints));
    this->client->tcp_hints.ai_family = AF_INET;
    this->client->tcp_hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(this->client->serverIP.c_str(), this->client->serverPort.c_str(), &this->client->tcp_hints, &this->client->tcp_res) != 0) {
        perror("Error getting address info");
        exit(1);
    }

    if (connect(this->client->tcp_sockfd, this->client->tcp_res->ai_addr, this->client->tcp_res->ai_addrlen) == ERROR) {
        perror("Error connecting to server");
        exit(1);
    }
    this->send();
    this->receive();
    freeaddrinfo(client->tcp_res);
    close(client->tcp_sockfd);
    this->handleReceive();
    return 0;
}

void TCPCommand::receive() {
    struct timeval tv;
    tv.tv_sec = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    // Set timeout for receiving data
    if(setsockopt(this->client->tcp_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
        exit(1);
    }

    std::string dataReceived = "";
    int n = 1, totalBytes = 0;

    while (n != 0) {
        char buffer[1024];
        n = read(this->client->tcp_sockfd, buffer, sizeof(buffer));

        printf("Received %d bytes\n", n);

        if (n < 0) {
            perror("Error receiving data");
            if(tv.tv_sec == CONNECTION_TIMEOUT) {
                perror("Connection timeout");
            }
            exit(1);
        }

        totalBytes += n;

        if (totalBytes > MAX_TCP_REPLY_SIZE) {
            perror("Error receiving data: reply too large");
            exit(1);
        }

        dataReceived.append(buffer, n);
    }
    this->data = dataReceived.substr(0, dataReceived.length() - 1);
}
