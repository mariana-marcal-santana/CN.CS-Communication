#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define PORT_FLAG "-p"

#define ERROR -1
#define INVALID_COMMAND_MSG "Invalid command"
#define HAS_STARTED_GAME "You have already started a game"
#define BUFFER_SIZE 128
#define BACKLOG 10

#define UDP 1
#define TCP 2

#define CONNECTION_TIMEOUT 600
#define MAX_FILE_SIZE 1024
#define MAX_TCP_REPLY_SIZE 3 + 3 + 24 + 4 + MAX_FILE_SIZE + 1 + 5 // RSS + status + fname + fsize + fdata + <spaces>
#define MAX_TRIES 8

#define START "SNG"
#define TRY "TRY"
#define SHOW_TRIALS "show_trials"
#define ST "st"
#define SCOREBOARD "scoreboard"
#define SB "sb"
#define QUIT "QUT"
#define EXIT "exit"
#define DEBUG "DBG"

#define OK "OK"
#define NOK "NOK"
#define ERR "ERR"
#define DUP "DUP"
#define INV "INV"
#define ENT "ENT"
#define ETM "ETM"
#define ACT "ACT"
#define FIN "FIN"
#define EMPTY "EMPTY"

#define SERVER_PORT "58052"

#endif