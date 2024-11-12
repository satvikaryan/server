# Compiler and flags
CC = gcc
CFLAGS = -Wall -pthread -g

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
WWW_DIR = www

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/request_handler.c \
       $(SRC_DIR)/thread_pool.c \
       $(SRC_DIR)/logger.c \
       $(SRC_DIR)/server.c \
       $(SRC_DIR)/config.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Output binary
TARGET = $(BIN_DIR)/webserver

# Default target
all: $(TARGET)

# Rule to build the server binary
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET)

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and binary
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Rebuild everything
rebuild: clean all

# Install the web server (optional)
install:
	cp $(TARGET) /usr/local/bin/

# Run the web server
run: $(TARGET)
	$(TARGET) -c config/server.conf

# Rule to display available targets
.PHONY: all clean rebuild install run
