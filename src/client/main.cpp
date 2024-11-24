#include "main.hpp"

int main(int argc, char** argv) {

    std::string serverIP = SERVER_IP;
    std::string serverPort = SERVER_PORT;

    for (int i = 0; i < argc; i++) {

        std::string flag = argv[i];

        if (flag == IP_FLAG) serverIP = argv[i + 1];
        else if (flag == PORT_FLAG) serverPort = argv[i + 1];
    }

    Client* client = new Client("", serverIP, serverPort);
    if (client->setupConnection() == ERROR) {
        perror("Error setting up connection");
        exit(1);
    }

    int exit = 0;
    while (!exit) {

        printf("\n> ");
        std::string input;
        getline(std::cin, input);
        std::istringstream iss(input);
        std::string arg;
        std::vector<std::string> args;
        while (iss >> arg) { args.push_back(arg); }

        Command* command = CommandHandler::createCommand(args);

        if (command == nullptr) {
            printf("%s: check command requirements.\n", INVALID_COMMAND_MSG);
            continue;
        }
        else {
            if (args[0] == START) { client->plid = args[1]; }
            if (args[0] == QUIT || args[0] == EXIT) { client->plid = ""; }

            command->client = std::unique_ptr<Client>(client);
            exit = command->execute();
            delete command;
        }
    }
    freeaddrinfo(client->res);
    close(client->sockfd);
    delete client;
    return 0;
}