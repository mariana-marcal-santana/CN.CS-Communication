#include "main.hpp"

int main (int argc, char *argv[]) {
    
    int errcode, out_fds, ret, newfd;
    char buffer[BUFFER_SIZE], prt_str[90], 
        host[NI_MAXHOST], service[NI_MAXSERV];
    socklen_t addrlen;
    fd_set inputs, testfds;
    struct timeval timeout;
    struct addrinfo hints, *res;
    struct sockaddr_in udp_useraddr, tcp_useraddr;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE|AI_NUMERICSERV;

    if ((getaddrinfo(NULL, SERVER_PORT, &hints, &res)) != 0)
        exit(1);

    // UDP socket
    int udp = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp == ERROR) {
        perror("UDP socket creation failed");
        exit(1);
    }

    if (bind(udp, res->ai_addr, res->ai_addrlen) == ERROR) {
        sprintf(prt_str, "Bind error UDP server\n");
        write(1, prt_str, strlen(prt_str)); // ???
        exit(1);
    }

    if (res != NULL) freeaddrinfo(res);

    // TCP socket
    int tcp = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp == ERROR) {
        perror("TCP socket creation failed");
        exit(1);
    }
    
    if (bind(tcp, res->ai_addr, res->ai_addrlen) == ERROR) {
        sprintf(prt_str, "Bind error TCP server\n");
        write(1, prt_str, strlen(prt_str)); // ???
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
    
    timeout.tv_sec = 1000;

    while (1) {
        
        testfds = inputs;
        out_fds = select(FD_SETSIZE, &testfds, (fd_set *) NULL, (fd_set *) NULL, (struct timeval *) &timeout);

        switch(out_fds) {
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
                    ret = recvfrom(udp, prt_str, 80, 0, (struct sockaddr *) &udp_useraddr, &addrlen);

                    if (ret >= 0) {

                        if (strlen(prt_str) > 0)
                            prt_str[ret - 1] = 0; // \0 ??

                        printf("UDP socket: %s\n", prt_str);
                        errcode = getnameinfo((struct sockaddr *) &udp_useraddr, addrlen, host, sizeof host, service, sizeof service, 0);
                        
                        if (errcode == 0)
                            printf("Sent by [%s:%s]\n",host,service);

                    }
                }
                if (FD_ISSET(tcp, &testfds)) {
                    
                    addrlen = sizeof(tcp_useraddr);

                    if ((newfd = accept(tcp, (struct sockaddr *) &tcp_useraddr, &addrlen)) == ERROR) {
                        perror("Accept error");
                        exit(1);
                    }

                    if ((ret = read(newfd, buffer, BUFFER_SIZE)) == ERROR) {
                        perror("Read error");
                        exit(1);
                    }

                    if (write(1, buffer, ret) == ERROR) {
                        perror("Write error");
                        exit(1);
                    }

                    close(newfd);
                }
        }
    }

    if (res != NULL) freeaddrinfo(res);
    close(udp);
    close(tcp);

    return 0;
}