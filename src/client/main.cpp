#include "main.hpp"

int main(int argc, char** argv) {

    std::string serverIP = SERVER_IP;
    std::string serverPort = SERVER_PORT;

    for (int i = 0; i < argc; i++) {

        std::string flag = argv[i];

        if (flag == IP_FLAG) serverIP = argv[i + 1];
        else if (flag == PORT_FLAG) serverPort = argv[i + 1];
    }

    std::string plid = "";

    Client* client = new Client(plid, serverIP, serverPort);
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

        if (args[1] == START) { plid = args[1]; }

        Command* command = CommandHandler::createCommand(args);

        if (args[1] == QUIT || args[1] == EXIT) { plid = ""; }

        if (command == nullptr) {
            perror(INVALID_COMMAND_MSG);
            continue;
        }
        else {
            command->client = std::unique_ptr<Client>(client);
            printf("Executing command...\n");
            exit = command->execute();
            delete command; 
        }
    }
    return 0;
}