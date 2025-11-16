CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -Werror \
           -Wshadow -Wpointer-arith -Wcast-align \
           -Wconversion -Wsign-conversion \
           -Wuninitialized -Wmaybe-uninitialized \
           -fstack-protector-strong -O2 -g \
           -std=c++20

SERVER_DIR = src/server
CLIENT_DIR = src/client
COMMON_DIR = src/common

# Pega todos os .cpp da pasta common
COMMON_SOURCES = $(wildcard $(COMMON_DIR)/*.cpp)

SERVER_SOURCES = $(wildcard $(SERVER_DIR)/*.cpp) $(COMMON_SOURCES)
CLIENT_SOURCES = $(wildcard $(CLIENT_DIR)/*.cpp) $(COMMON_SOURCES)

SERVER_TARGET = ES
CLIENT_TARGET = user

.PHONY: all clean run-server run-client

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CLIENT_TARGET): $(CLIENT_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(SERVER_TARGET) $(CLIENT_TARGET)
