#include "protocol.hpp"

Client::~Client(){
    if(this->sockfd >= 0){
        close(this->sockfd);
    }
}

bool Client::hasStartedGame() {
    return this->plid >= 0;
}

// UDPClient
UDPClient::UDPClient(std::string serverIP, int serverPort) {

    this->serverIP = serverIP;
    this->serverPort = serverPort;

    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->serverPort);
    this->serverAddr.sin_addr.s_addr = inet_addr(this->serverIP.c_str());
}

int UDPClient::sendData(const std::string& data) {

    int n = sendto(this->sockfd, data.c_str(), data.length(), 0, (struct sockaddr*) &this->serverAddr, sizeof(this->serverAddr));

    if (n < 0) {
        perror("Error sending data");
        exit(1);
    }

    return n;
}

std::string UDPClient::receiveData() {
    struct timeval tv;
    tv.tv_sec = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    // Set timeout for receiving data
    if(setsockopt(this->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
        exit(1);
    }

    char buffer[128];
    socklen_t len = sizeof(this->serverAddr);
    int n = recvfrom(this->sockfd, buffer, 128, 0, (struct sockaddr*) &this->serverAddr, &len);

    if(n < 0) {
        perror("Error receiving data");
        if(tv.tv_sec == CONNECTION_TIMEOUT) {
            perror("Connection timeout");
        }
        exit(1);
    }

    // Set to n-1 because of the \n, removing \n helps with parsing and printing
    buffer[n - 1] = '\0';

    std::string dataReceived = std::string(buffer);

    return dataReceived;
}

// TCPClient
TCPClient::TCPClient(std::string serverIP, int serverPort) {
    this->serverIP = serverIP;
    this->serverPort = serverPort;

    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (this->sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    this->serverAddr.sin_family = AF_INET;
    this->serverAddr.sin_port = htons(this->serverPort);
    this->serverAddr.sin_addr.s_addr = inet_addr(this->serverIP.c_str());

    if (connect(this->sockfd, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }
}

int TCPClient::sendData(const std::string &data) {
    //if(TCP_VERBOSE) printf("Sending data: %s", data.c_str());

    int n = write(this->sockfd, data.c_str(), data.length());

    if (n < 0) {
        perror("Error sending data");
        exit(1);
    }

    return n;
}

std::string TCPClient::receiveData() {
    struct timeval tv;
    tv.tv_sec = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    // Set timeout for receiving data
    if(setsockopt(this->sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("Error setting timeout");
        exit(1);
    }

    std::string dataReceived = "";

    // n is the number of bytes read, set to 1 to enter the loop
    int n = 1;
    int totalBytes = 0;

    // loops until there is no more data to read
    // add data being read to dataReceived string
    while(n != 0) {
        char buffer[128];
        n = read(this->sockfd, buffer, sizeof(buffer));

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

    // remove \n from the end of the string
    dataReceived = dataReceived.substr(0, dataReceived.length() - 1);

    //if(TCP_VERBOSE) printf("Received data: %s\n", dataReceived.c_str());

    return dataReceived;
}