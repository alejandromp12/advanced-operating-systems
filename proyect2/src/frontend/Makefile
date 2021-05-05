# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-c -I.
LFLAGS= -lm

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

BUILD_DIR=build_tmp
BIN_DIR=$(BUILD_DIR)/bin
LIB_DIR=$(BUILD_DIR)/lib
STATIC_COMPONENT=$(LIB_DIR)/libgui.a
FILE_NAME=gui
SRC_DIR=src
TARGET_FILE=$(SRC_DIR)/$(FILE_NAME).c
OBJECT_FILE=$(BIN_DIR)/$(FILE_NAME).o

$(STATIC_COMPONENT): $(OBJECT_FILE)
	ar rcs $@ $^

$(OBJECT_FILE): $(TARGET_FILE) $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)
	$(CC) $(CFLAGS) $(TARGET_FILE) -o $@ $(GTKLIB)

$(LIB_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf build/
