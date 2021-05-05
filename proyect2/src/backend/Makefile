# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-c -I. -g -Wall
LFLAGS=-lrt -lpthread -lm

BUILD_DIR=build_tmp
BIN_DIR=$(BUILD_DIR)/bin
LIB_DIR=$(BUILD_DIR)/lib
STATIC_COMPONENT=$(LIB_DIR)/libbackend.a
SRC_DIR=src
OBJECT_FILES=$(BIN_DIR)/producer.o $(BIN_DIR)/consumer.o $(BIN_DIR)/finisher.o $(BIN_DIR)/factory.o $(BIN_DIR)/common.o
FILE_NAMES=producer consumer finisher factory common

build: $(STATIC_COMPONENT)

define BUILD_OBJECT
.PHONY: $(1).o
$(1).o: $(2).c $(BUILD_DIR) $(BIN_DIR) $(LIB_DIR)
	$(CC) $(CFLAGS) $(2).c -o $$@ $(LFLAGS)
endef # BUILD_OBJECT

# Build .o files
$(foreach file,$(FILE_NAMES),$(eval $(call BUILD_OBJECT,$$(BIN_DIR)/$(file),$$(SRC_DIR)/$(file))))


$(STATIC_COMPONENT): $(OBJECT_FILES)
	ar rcs $@ $^

$(LIB_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)/

