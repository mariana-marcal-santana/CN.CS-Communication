#include "command.hpp"

int Command::execute() {
    this->send();
    this->receive();
    return 0;
}

void Command::send() {
    std::string dataToSend = this->formatData();
    this->Client->sendData(dataToSend);
}

void Command::setNetworkClient(std::string serverIp, int serverPort) {
    int networkType = this->networkType;

    if(networkType == UDP) {
        this->Client = std::make_unique<UDPClient>(serverIp, serverPort);
    }

    else if(networkType == TCP) {
        this->Client = std::make_unique<TCPClient>(serverIp, serverPort);
    }
}