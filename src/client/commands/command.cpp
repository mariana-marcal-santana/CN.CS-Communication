#include "command.hpp"

void UDPCommand::send() {

    std::string dataToSend = this->formatData();
    printf("%s", dataToSend.c_str());

    if (sendto(this->client->sockfd, dataToSend.c_str(), dataToSend.length(), 0,
        this->client->res->ai_addr, this->client->res->ai_addrlen) < 0) { 
        perror("Error sending data");
        exit(1);
    }
}

int UDPCommand::execute() {
    printf("%s", "execUDP\n");
    this->send();
    this->receive();
    this->handleReceive();
    return 0;
}

void UDPCommand::receive() {
    struct timeval tv;
    tv.tv_sec = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    // Set timeout for receiving data
    if (setsockopt(this->client->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
        exit(1);
    }

    char buffer[128];
    socklen_t len = sizeof(this->client->addr);
    int n = recvfrom(this->client->sockfd, buffer, 128, 0, (struct sockaddr*) &this->client->addr, &len);

    if(n < 0) {
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
    //printf("Sending data: %s\n", dataToSend.c_str());

    if (write(this->client->sockfd, dataToSend.c_str(), dataToSend.length()) < 0) {
        perror("Error sending data");
        exit(1);
    }
}

int TCPCommand::execute() {
    printf("%s", "execTCP\n");
    // connectttt
    this->send();
    this->receive();
    this->handleReceive();
    return 0;
}

void TCPCommand::receive() {
    struct timeval tv;
    tv.tv_sec = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    // Set timeout for receiving data
    if(setsockopt(this->client->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
        exit(1);
    }

    std::string dataReceived = "";

    // n is the number of bytes read, set to 1 to enter the loop
    int n = 1, totalBytes = 0;

    // loops until there is no more data to read
    // add data being read to dataReceived string
    while(n != 0) {
        char buffer[128];
        n = read(this->client->sockfd, buffer, sizeof(buffer));

        if(n < 0) {
            perror("Error receiving data");
            if(tv.tv_sec == CONNECTION_TIMEOUT) {
                perror("Connection timeout");
            }
            exit(1);
        }

        totalBytes += n;

        if(totalBytes > MAX_TCP_REPLY_SIZE) {
            perror("Error receiving data: reply too large");
            exit(1);
        }

        dataReceived.append(buffer, n);
    }

    this->data = dataReceived.substr(0, dataReceived.length() - 1);
}
