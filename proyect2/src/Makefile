# the compiler: gcc for C program, define as g++ for C++
CC=gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS=-I. -g -Wall
LFLAGS=
GTKLIB=`pkg-config --cflags --libs gtk+-3.0`


BIN_DIR=bin
LIB_BACKEND=backend/build_tmp/lib/libbackend.a
LIB_GUI=frontend/build_tmp/lib/libgui.a
APP=$(BIN_DIR)/product_consumer

build: backend frontend $(APP)

.PHONY: backend frontend
backend:
	$(MAKE) -C $@

frontend:
	$(MAKE) -C $@

$(APP): $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ main.c $(LIB_BACKEND) $(LIB_GUI) $(LFLAGS) $(GTKLIB)

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf bin/
	cd backend && make clean
	cd frontend && make clean
