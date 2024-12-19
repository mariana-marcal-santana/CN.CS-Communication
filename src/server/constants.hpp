#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#define PORT_FLAG "-p"
#define VERBOSE_FLAG "-v"
#define SERVER_PORT "58052"

#define DB_PATH "db/"
#define DB_GAMES_PATH "db/GAMES"
#define DB_SCORES_PATH "db/SCORES"

#define ERROR -1
#define INVALID_COMMAND_MSG "Invalid command"
#define BUFFER_SIZE 128
#define BACKLOG 5

#define UDP 1
#define TCP 2

#define GAME_TIMEOUT 600
#define MAX_FILE_SIZE 1024
#define MAX_TRIES 8

#define START "SNG"
#define TRY "TRY"
#define SHOW_TRIALS "STR"
#define SCOREBOARD "SSB"
#define QUIT_EXIT "QUT"
#define DEBUG "DBG"

#define START_RESP "RSG"
#define TRY_RESP "RTR"
#define SHOW_TRIALS_RESP "RTS"
#define SCOREBOARD_RESP "RSS"
#define QUIT_EXIT_RESP "RQT"
#define DEBUG_RESP "RDB"

#define OK "OK"
#define NOK "NOK"
#define ERR "ERR\n"
#define DUP "DUP"
#define INV "INV"
#define ENT "ENT"
#define ETM "ETM"
#define ACT "ACT"
#define FIN "FIN"
#define EMPTY "EMPTY"

#endif