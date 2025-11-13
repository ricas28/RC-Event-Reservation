CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

SERVER_DIR = src/server
CLIENT_DIR = src/client
BIN_DIR = bin

SERVER_SOURCES = $(wildcard $(SERVER_DIR)/*.c)
CLIENT_SOURCES = $(wildcard $(CLIENT_DIR)/*.c)

SERVER_TARGET = $(BIN_DIR)/server
CLIENT_TARGET = $(BIN_DIR)/client

.PHONY: all clean

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_TARGET): $(CLIENT_SOURCES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(BIN_DIR)
