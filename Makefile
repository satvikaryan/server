CC = gcc
CFLAGS = -Wall -pthread
TARGET = web_server

SRC = src/main.c src/request_handler.c src/thread_pool.c src/logger.c
INC = -Iinclude

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(INC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
