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
    while(!exit) {

        printf("\n> ");

        std::string input;
        getline(std::cin, input);

        Command* command;

        command = CommandHandler::createCommand(input);

        // There is no corresponding command for the input
        // or the command format is invalid
        if (command == nullptr) {
            printf("%s\n", INVALID_COMMAND_MSG);
            continue;
        }

        exit = command->execute();

        // try {
        //     command = CommandHandler::createCommand(input);

        //     // There is no corresponding command for the input
        //     // or the command format is invalid
        //     if (command == nullptr) {
        //         printf("%s\n", INVALID_COMMAND_MSG);
        //         continue;
        //     }

        //     command->setNetworkClient(serverIP, serverPort);

        //     command->setClientState(&clientState);

        //     exit = command->execute();
        // } catch (const CostumError& e) {
        //     printf("%s\n", e.what());
        // } catch (const exception& e) {
        //     printf("%s\n", string(UNKNOW_ERROR_MSG).c_str());
        // }

        if (command != nullptr) {
            delete command;
        }
    }

    return 0;
}