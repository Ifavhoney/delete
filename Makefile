UNAME := $(shell uname)
CC = gcc -Iinclude/
CFLAGS = -Wall -std=c11 -g
LDFLAGS= -L.

ifeq ($(UNAME), Linux)
	INC_PATH = /usr/include/libxml2
endif
ifeq ($(UNAME), Darwin)
	INC_PATH = /System/Volumes/Data/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/libxml2
endif

all: parser

parser: bin/libsvgparse.so

bin/libsvgparse.so: LinkedListAPI.o SVGParser.o
	$(CC) -shared -o bin/libsvgparse.so LinkedListAPI.o SVGParser.o -lxml2 

LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c

SVGParser.o: src/SVGParser.c include/SVGParser.h
	$(CC) $(CFLAGS) -I$(INC_PATH) -c -fpic src/SVGParser.c -lxml2 

main: all
	$(CC) $(CFLAGS) -I$(INC_PATH) -fpic src/main.c -o $@ -lxml2 
	
clean: 
	rm main -rf *.o ./bin/*.so
