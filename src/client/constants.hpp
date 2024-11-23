#ifndef CLIENT_CONSTANTS_H
#define CLIENT_CONSTANTS_H

#define IP_FLAG "-n"
#define PORT_FLAG "-p"
#define SERVER_IP "?"
#define SERVER_PORT 58000 // + GN !!
#define CONNECTION_TIMEOUT 600
#define MAX_FILE_SIZE 1024
#define MAX_TCP_REPLY_SIZE 3 + 3 + 24 + 4 + MAX_FILE_SIZE + 1 + 5 // RSS + status + fname + fsize + fdata + <spaces>

#define START "start"
#define TRY "try"
#define SHOW_TRIALS "show_trials"
#define ST "st"
#define SCOREBOARD "scoreboard"
#define SB "sb"
#define QUIT "quit"
#define EXIT "exit"
#define DEBUG "debug"

#define OK "OK"
#define NOK "NOK"
#define ERR "ERR"
#define DUP "DUP"
#define INV "INV"
#define ENT "ENT"
#define ETM "ETM"


#define UDP 1
#define TCP 2

#define INVALID_COMMAND_MSG "Invalid command"
#define HAS_STARTED_GAME "You have already started a game"

#endif