#include "showtrials.hpp"

void ShowTrialsCommand::receive() {
    // Set timeout for receiving data
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(this->client->tcp_sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == ERROR) {
        perror("Error setting timeout");
        exit(1);
    }

    int n;

    char buf[2];
    memset(buf, '\0', sizeof(buf));

    std::string cmd;
    while ((n = read(this->client->tcp_sockfd, buf, 1)))
    {
        if (n == ERROR) {
            perror("Error receiving data");
            exit(1);
        }
        buf[1] = '\0';
        if (strcmp(buf, " ") == 0) {
            break;
        }
        cmd.append(buf, 1);
    }
    
    std::string status;
    while ((n = read(this->client->tcp_sockfd, buf, 1)))
    {
        if (n == ERROR) {
            perror("Error receiving data");
            exit(1);
        }
        buf[1] = '\0';
        if (strcmp(buf, " ") == 0 || strcmp(buf, "\n") == 0) {
            break;
        }
        status.append(buf, 1);
    }

    printf("status:|%s|", status.c_str());

    if (strcmp(status.c_str(), NOK) == 0) {
        this->data = std::string(cmd) + " " + std::string(status);
        return;
    }

    std::string fname;
    while ((n = read(this->client->tcp_sockfd, buf, 1)))
    {
        if (n == ERROR) {
            perror("Error receiving data");
            exit(1);
        }
        buf[1] = '\0';
        if (strcmp(buf, " ") == 0) {
            break;
        }
        fname.append(buf, 1);
    }

    std::string fsize;
    while ((n = read(this->client->tcp_sockfd, buf, 1)))
    {
        if (n == ERROR) {
            perror("Error receiving data");
            exit(1);
        }
        buf[1] = '\0';
        if (strcmp(buf, " ") == 0) {
            break;
        }
        fsize.append(buf, 1);
    }    

    char buffer[BUFFER_SIZE];
    std::string fdata = "";
    int size;
    try {
        size = std::stoi(fsize);
    } catch (const std::invalid_argument& e) {
        std::cerr << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    } catch (const std::out_of_range& e) {
        std::cerr << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    }

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

    this->data = std::string(cmd) + " " + std::string(status) + " " + std::string(fname) + " " + std::string(fsize);
}

void ShowTrialsCommand::handleReceive() { // RST status [Fname Fsize Fdata]

    printf("Received data: %s\n", this->data.c_str());

    std::istringstream iss(this->data);
    std::string arg;
    std::vector<std::string> args;
    while (iss >> arg) {
        args.push_back(arg);
    }

    if (args.size() == 1 && args[0] == ERR) {
        std::cout << INVALID_COMMAND_MSG << std::endl;
        return;
    }
    
    if (args.size() != 4 && args.size() != 2) {
        std::cout << UNPARSEABLE_MSG_SERVER << std::endl;
        return;
    }

    if (args[0] != RST) {
        std::cout << WRONG_COMMAND_MSG_SERVER << std::endl;
        return;
    }

    else if (args[1] == NOK) {
        std::cout << "There're no games for this player." << std::endl;
        return;
    }

    this->client->playing = args[1] == ACT ? true : false;

    printf("Trials:\n");

    std::ifstream trials(args[2]);
    if (!trials.is_open()) {
        perror("Unable to open file");
        exit(1);
    }

    std::cout << "File stored in current directory - file name " + args[2] + " file size " + args[3] << std::endl;

    std::string line;
    while (std::getline(trials, line)) {
        std::cout << line << std::endl;
    }    
    trials.close();
}

std::string ShowTrialsCommand::formatData() {
    return "STR " + this->client->plid + "\n";
}   

bool ShowTrialsCommand::shouldSend() {
    bool res = this->client->plid != "";
    if (!res) {
        std::cout << "You must set a plid." << std::endl;
    }
    return res;
}