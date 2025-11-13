CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Werror \
         -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes \
         -Wmissing-prototypes -Wconversion -Wsign-conversion \
         -Wuninitialized -Wmaybe-uninitialized \
         -fstack-protector-strong -O2 -g

SERVER_DIR = src/server
CLIENT_DIR = src/client

SERVER_SOURCES = $(wildcard $(SERVER_DIR)/*.c)
CLIENT_SOURCES = $(wildcard $(CLIENT_DIR)/*.c)

SERVER_TARGET = server
CLIENT_TARGET = client

.PHONY: all clean run-server run-client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT_TARGET): $(CLIENT_SOURCES)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)

run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

run-client: $(CLIENT_TARGET)
	./$(CLIENT_TARGET)
