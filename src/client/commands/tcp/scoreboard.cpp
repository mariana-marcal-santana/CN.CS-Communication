#include "scoreboard.hpp"

void ScoreboardCommand::receive() {

    // Set timeout for receiving data
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(this->client->tcp_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == ERROR) {
        perror("Error setting timeout");
        exit(1);
    }

    int n;

    char id_status[10] = "";
    if ((n = read(this->client->tcp_sockfd, id_status, 9)) == ERROR) {
        perror("Error receiving data (id_status)");
        exit(1);
    }
    id_status[n] = '\0';

    if (strstr(id_status, EMPTY) != nullptr) {
        this->data = EMPTY;
        return;
    }

    char fname[25];
    if ((n = read(this->client->tcp_sockfd, fname, 24)) == ERROR) {
        perror("Error receiving data (fname)");
        exit(1);
    }
    fname[n] = '\0';

    char fsize[4];
    if ((n = read(this->client->tcp_sockfd, fsize, 4)) == ERROR) {
        perror("Error receiving data (fsize)");
        exit(1);
    }
    fsize[n] = '\0';

    char buffer[BUFFER_SIZE];
    std::string fdata = "";
    int size = std::stoi(fsize);

    while (size > 0) {
        if ((n = read(this->client->tcp_sockfd, buffer, (size < BUFFER_SIZE) ? size : BUFFER_SIZE)) == ERROR) {
            perror("Error writing to client (fdata)");
            exit(1);
        }
        size -= n;
        fdata.append(buffer, n);
    }

    std::ofstream scoreboard(fname);
    if (!scoreboard.is_open()) {
        perror("Unable to open file");
        exit(1);
    }
    scoreboard << fdata;
    scoreboard.close();

    this->data = fdata;
    std::string dataReceived = std::string(id_status) + " " + std::string(fname) + " " + std::string(fsize);
    printf("Received data: %s\n", dataReceived.c_str());
}

void ScoreboardCommand::handleReceive() {

    if (this->data == EMPTY) {
        std::cout << "No game has been won yet." << std::endl;
        return;
    }
    
    printf("Scoreboard:\n %s", this->data.c_str());
}

std::string ScoreboardCommand::formatData() {
    return "SSB\n";
}   