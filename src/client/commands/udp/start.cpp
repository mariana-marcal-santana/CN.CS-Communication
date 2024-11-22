#include "start.hpp"

void StartCommand::send() {
    std::string dataToSend = this->formatData();
    this->Client->sendData(dataToSend);
}

void StartCommand::receive() {}

int StartCommand::execute() {}