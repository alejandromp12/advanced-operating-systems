# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-g -Wall
LFLAGS=-lpthread -lm

BIN_DIR=bin
SRC_DIR=src
TARGET_FILE_NAME=$(SRC_DIR)/app
TARGET_APP=$(BIN_DIR)/shortTask1

build: $(TARGET_FILE_NAME).c $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET_APP) $(TARGET_FILE_NAME).c $(LFLAGS)

clean:
	rm -f bin/*

$(BIN_DIR):
	mkdir $(BIN_DIR)


