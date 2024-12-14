#include "main.hpp"

int main (int argc, char *argv[]) {

    std::string serverPort = (argc > 1 && std::string(argv[1]) == PORT_FLAG) ? argv[2] : SERVER_PORT;
    bool verbose = (argc > 1 && (std::string(argv[1]) == VERBOSE_FLAG || std::string(argv[3]) == VERBOSE_FLAG));
    
    int n, out_fds, newfd;
    char buffer[BUFFER_SIZE], prt_str[90], 
        host[NI_MAXHOST], service[NI_MAXSERV];
    socklen_t addrlen;
    fd_set inputs, testfds;
    struct timeval timeout;
    struct addrinfo udp_hints, tcp_hints, *udp_res, *tcp_res;
    struct sockaddr_in udp_useraddr, tcp_useraddr;
    pid_t pid;
    ssize_t nw;

    memset(&udp_hints, 0, sizeof udp_hints);
    udp_hints.ai_family = AF_INET;
    udp_hints.ai_socktype = SOCK_DGRAM;
    udp_hints.ai_flags = AI_PASSIVE;

    if ((getaddrinfo(NULL, serverPort.c_str(), &udp_hints, &udp_res)) != 0)
        exit(1);

    memset(&tcp_hints, 0, sizeof tcp_hints);
    tcp_hints.ai_family = AF_INET;
    tcp_hints.ai_socktype = SOCK_STREAM;
    tcp_hints.ai_flags = AI_PASSIVE;

    if ((getaddrinfo(NULL, serverPort.c_str(), &tcp_hints, &tcp_res)) != 0)
        exit(1);

    // UDP socket
    int udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp == ERROR) {
        perror("UDP socket creation failed");
        exit(1);
    }

    if (bind(udp, udp_res->ai_addr, udp_res->ai_addrlen) == ERROR) {
        perror("Bind error UDP server");
        exit(1);
    }

    // TCP socket
    int tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp == ERROR) {
        perror("TCP socket creation failed");
        exit(1);
    }

    struct timeval tcp_timeout;
    tcp_timeout.tv_sec = 0;
    tcp_timeout.tv_usec = 500000;  // 0.5 seconds
    if (setsockopt(tcp, SOL_SOCKET, SO_RCVTIMEO, (char *)&tcp_timeout, sizeof(tcp_timeout)) < 0) {
        perror("Failed to set socket options");
        exit(1);
    }
    
    if (bind(tcp, tcp_res->ai_addr, tcp_res->ai_addrlen) == ERROR) {
        perror("Bind error TCP server");
        exit(1);
    }

    if (listen(tcp, BACKLOG) == ERROR) {
        perror("Listen error");
        exit(1);
    }

    FD_ZERO(&inputs);
    FD_SET(0, &inputs);
    FD_SET(udp, &inputs);
    FD_SET(tcp, &inputs);
    
    printf("Server is running\n");
    
    while (1) {
        testfds = inputs;

        memset((void *)&timeout,0,sizeof(timeout));
        timeout.tv_sec = 1000;
        out_fds = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);

        switch (out_fds) {
            case 0:
                printf("Timeout\n");
                break;
            case ERROR:
                perror("Error in select");
                exit(1);
            
            default:
                if (FD_ISSET(0, &testfds)) {
                    fgets(buffer, 50, stdin);
                    printf("Input at keyboard: %s\n", buffer);
                }

                if (FD_ISSET(udp, &testfds)) {

                    addrlen = sizeof(udp_useraddr);
                    std::memset(prt_str, '\0', sizeof(prt_str   ));
                    if (recvfrom(udp, prt_str, 80, 0, (struct sockaddr *) &udp_useraddr, &addrlen) == ERROR) {
                        perror("Recvfrom error");
                        exit(1);
                    }

                    if (verbose) {
                        if (getnameinfo((struct sockaddr *) &udp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0) == 0) {
                            printf("UDP command sent by [%s:%s]: %s", host, service, prt_str);
                            fflush(stdout);
                        }
                    }
                    
                    Command* command = CommandHandler::createCommand(prt_str);
                    std::string response = command != nullptr ? command->execute() : ERR;
                    
                    if (sendto(udp, response.c_str(), response.size(), 0, (struct sockaddr*)&udp_useraddr, addrlen) == ERROR) {
                        perror("Sendto error");
                        exit(1);
                    }
                }

                if (FD_ISSET(tcp, &testfds)) {

                    addrlen = sizeof(tcp_useraddr);
                    do newfd = accept(tcp, (struct sockaddr *) &tcp_useraddr, &addrlen);
                    while (newfd == -1 && errno == EINTR);

                    if (newfd == -1) {
                        perror("Accept error");
                        exit(1);
                    }

                    // fork
                    if ((pid = fork()) == ERROR) {
                        perror("Fork error");
                        exit(1);
                    }
                    else if (pid == 0) {

                        printf("Child process\n");

                        std::string received_data;

                        char buf[2];
                        memset(buf, '\0', sizeof(buf));
                        while ((n = read(newfd, buffer, 1)) > 0) {
                            if (n == ERROR) {
                                perror("Error reading from client");
                                exit(1);
                            }
                            received_data.append(buffer, n);
                            if (buffer[0] == '\n') {
                                break;
                            }
                        }

                        if (verbose) {
                            if (getnameinfo((struct sockaddr *) &tcp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0) == 0) {
                                printf("TCP command sent by [%s:%s]: %s", host, service, received_data.c_str());
                                fflush(stdout);
                            }
                        }

                        Command* command = CommandHandler::createCommand(received_data);

                        std::string resp;
                        std::vector<std::string> response;
                        if (command == nullptr) {
                            resp = "ERR\n";
                        }
                        else {
                            resp = command->execute();
                            delete command;
                        }

                        printf("Response: %s\n", resp.c_str());

                        if ((nw = write(newfd, resp.c_str(), resp.size())) == ERROR) {
                            perror("Error writing to client");
                            exit(1);
                        }


                        // printf("id_status: %s\n", response[0].c_str());
                        // if ((nw = write(newfd, response[0].c_str(), response[0].size())) == ERROR) {
                        //     perror("Error writing to client (id and status)");
                        //     exit(1);
                        // }

                        // if (response.size() > 1) {
                            
                        //     char fname[24];
                        //     memset(fname, '\0', sizeof(fname));
                        //     strcpy(fname, response[1].c_str());
                        //     if ((nw = write(newfd, fname, 24)) == ERROR) {
                        //         perror("Error writing to client (fname)");
                        //         exit(1);
                        //     }

                        //     char fsize[4];
                        //     memset(fsize, '\0', sizeof(fsize));
                        //     strcpy(fsize, response[2].c_str());
                        //     if ((nw = write(newfd, fsize, 4)) == ERROR) {
                        //         perror("Error writing to client (fsize)");
                        //         exit(1);
                        //     }

                        //     int size = std::stoi(response[2]);
                        //     char *fdata_ptr = (char *)response[3].c_str();

                        //     while (size > 0) {
                        //         if ((nw = write(newfd, fdata_ptr, (size < BUFFER_SIZE) ? size : BUFFER_SIZE)) == ERROR) {
                        //             perror("Error writing to client (fdata)");
                        //             exit(1);
                        //         }
                        //         size -= nw; 
                        //         fdata_ptr += nw;
                        //     }
                        // }
                        close(newfd);
                        exit(EXIT_SUCCESS);
                    }
                    //do ret = close(newfd);
                    // while (ret == ERROR && errno == EINTR);
                    // if (ret == ERROR)
                    //     exit(1);
                }
        }
    }

    freeaddrinfo(udp_res);
    freeaddrinfo(tcp_res);
    close(udp);
    close(tcp);

    return 0;
}