# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-I. -g -Wall
LFLAGS=

BIN_DIR=bin
LIB_BACKEND=backend/build/lib/liblottery.a
APP=$(BIN_DIR)/lottery_threads
 
build: backend $(APP)

.PHONY: backend
backend:
	$(MAKE) -C $@

$(APP): $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ main.c $(LIB_BACKEND) $(LFLAGS)

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf bin/
	cd backend && make clean
