# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-c -I.
LFLAGS=

BUILD_DIR=build
BIN_DIR=$(BUILD_DIR)/bin
LIB_DIR=$(BUILD_DIR)/lib
STATIC_COMPONENT=$(LIB_DIR)/liblottery.a
FILE_NAME=threadsManager
SRC_DIR=src
TARGET_FILE=$(SRC_DIR)/$(FILE_NAME).c
OBJECT_FILE=$(BIN_DIR)/$(FILE_NAME).o

$(STATIC_COMPONENT): $(OBJECT_FILE)
	ar rcs $@ $^

$(OBJECT_FILE): $(TARGET_FILE) $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)
	$(CC) $(CFLAGS) $(TARGET_FILE) -o $@

$(LIB_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf build/

