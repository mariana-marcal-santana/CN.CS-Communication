#include "main.hpp"

int main (int argc, char *argv[]) {

    std::string serverPort = (argc > 1 && std::string(argv[1]) == PORT_FLAG) ? argv[2] : SERVER_PORT;
    bool verbose = (argc > 1 && (std::string(argv[1]) == VERBOSE_FLAG || std::string(argv[3]) == VERBOSE_FLAG));

    printf("verbose: %d\n", verbose);

    int out_fds, ret, newfd;
    char *ptr, buffer[BUFFER_SIZE], prt_str[90], 
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
        //printf("testfds byte: %d\n",((char *)&testfds)[0]);
        memset((void *)&timeout,0,sizeof(timeout));
        timeout.tv_sec = 1000;
        out_fds = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);

        //printf("testfds byte: %d\n",((char *)&testfds)[0]);
        printf("out_fds: %d\n", out_fds);

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

                    printf("UDPCommand received\n");

                    addrlen = sizeof(udp_useraddr);
                    if (recvfrom(udp, prt_str, 80, 0, (struct sockaddr *) &udp_useraddr, &addrlen) == ERROR) {
                        perror("Recvfrom error");
                        exit(1);
                    }

                    if (verbose) {
                        if (getnameinfo((struct sockaddr *) &udp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0) == 0) {
                            printf("Sent by [%s:%s]: %s", host, service, prt_str);
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

                    printf("TCPCommand received\n");
                    
                    addrlen = sizeof(tcp_useraddr);
                    do newfd = accept(tcp, (struct sockaddr *) &tcp_useraddr, &addrlen);
                    while (newfd == -1 && errno == EINTR);

                    if(newfd == -1) {
                        perror("Accept error");
                        exit(1);
                    }

                    // fork
                    printf("Forking\n");
                    if ((pid = fork()) == ERROR) {
                        perror("Fork error");
                        exit(1);
                    }
                    else if (pid == 0) {

                        printf("Child process\n");

                        std::string received_data;

                        while (true) {
                            memset(buffer, 0, sizeof(buffer));
                            int bytes_read = read(newfd, buffer, sizeof(buffer) - 1);
                            if (bytes_read <= 0) {
                                std::cout << "Client disconnected or error occurred." << std::endl;
                                break;
                            }
                            received_data += buffer;
                        }
                        
                        // while ((n = read(newfd, buffer, BUFFER_SIZE)) != 0) {
                        //     printf("%ld\n", n);
                        //     fflush(stdout);
                        //     if (n == ERROR) {
                        //         perror("Read error");
                        //         exit(1);
                        //     }
                        //     else if (n == 0) {
                        //         break;
                        //     }
                        //     received_data.append(buffer, n);
                        // }

                        printf("finished reading\n");

                        if (verbose) {
                            if (getnameinfo((struct sockaddr *) &tcp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0) == 0) {
                                printf("Sent by [%s:%s]: %s", host, service, prt_str);
                                fflush(stdout);
                            }
                        }

                        Command* command = CommandHandler::createCommand(received_data);
                        printf("ok");

                        std::string response;
                        if (command == nullptr) {
                            response = "ERR\n";
                        }
                        else {
                            response = command->execute();
                            delete command;
                        }

                        //ptr=&buffer[0];
                        int size = response.size();
                        ptr = (char *)response.c_str();

                        printf("ok2");

                        while (size > 0) {
                            if ((nw = write(newfd, ptr, BUFFER_SIZE)) == ERROR) {
                                perror("Error writing to client");
                                exit(1);
                            }
                            size-=nw; 
                            ptr+=nw;
                        }
                        close(newfd);
                        exit(0);
                    }
                    do ret = close(newfd);
                    while (ret == ERROR && errno == EINTR);
                    if (ret == ERROR)
                        exit(1);

                }
        }
    }

    freeaddrinfo(udp_res);
    freeaddrinfo(tcp_res);
    close(udp);
    close(tcp);

    return 0;
}