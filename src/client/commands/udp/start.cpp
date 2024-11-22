#include "start.hpp"

void StartCommand::send() {
    std::string dataToSend = this->formatData();
    this->networkClient->sendData(dataToSend);
}
void receive() override;
int execute() override;