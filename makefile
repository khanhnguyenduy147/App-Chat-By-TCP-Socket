CC = gcc
CFLAGS = -Wall -pthread
CLIENT_SRC_DIR = Client/src
SERVER_SRC_DIR = Server/src
CLIENT_TARGET = Client/src/client_chat
SERVER_TARGET = Server/src/server_chat

all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(wildcard $(CLIENT_SRC_DIR)/*.c)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_TARGET): $(wildcard $(SERVER_SRC_DIR)/*.c)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET)
