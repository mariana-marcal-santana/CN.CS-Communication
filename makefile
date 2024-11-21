CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

CLIENT_DIR = src/client
SERVER_DIR = src/server

CLIENT_SOURCES = $(shell find $(CLIENT_DIR) -name '*.cpp')
SERVER_SOURCES = $(shell find $(SERVER_DIR) -name '*.cpp')

CLIENT_EXEC = ./client
SERVER_EXEC = ./server

all: clean $(CLIENT_EXEC) $(SERVER_EXEC)

user: clean-user $(CLIENT_EXEC)

server: clean-server $(SERVER_EXEC)

$(CLIENT_EXEC):
	$(CXX) $(CXXFLAGS) $(CLIENT_SOURCES) -o $(CLIENT_EXEC)

$(SERVER_EXEC):
	$(CXX) $(CXXFLAGS) $(SERVER_SOURCES) -o $(SERVER_EXEC)

debug:
	@echo "Client Sources: $(CLIENT_SOURCES)"
	@echo "Server Sources: $(SERVER_SOURCES)"

clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC)

clean-user:
	rm -f $(CLIENT_EXEC)

clean-server:
	rm -f $(SERVER_EXEC)

.PHONY: all clean debug