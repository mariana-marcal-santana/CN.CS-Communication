#include "main.hpp"
#include "commands/commandHandler.hpp"
#include <iostream>

int main(int argc, char** argv) {

    std::string serverIP = SERVER_IP;
    int serverPort = SERVER_PORT;

    for (int i = 0; i < argc; i++) {

        std::string flag = argv[i];

        if (flag == IP_FLAG) serverIP = argv[i + 1];
        else if (flag == PORT_FLAG) serverPort = atoi(argv[i + 1]);
    }

    int exit = 0;
    while (!exit) {

        printf("\n> ");
        std::string input;
        getline(std::cin, input);

        Command* command = CommandHandler::createCommand(input);

        command->setNetworkClient(serverIP, serverPort);

        //command->setClientState(&clientState);

        if (command == nullptr) {
            perror(INVALID_COMMAND_MSG);
            continue;
        }
        else { 
            exit = command->execute();
            delete command; 
        }
    }
    return 0;
}