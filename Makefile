all: parser main

parser: bin/libsvgparse.so

bin/libsvgparse.so: LinkedListAPI.o SVGParser.o 
    $(CC) -shared -o bin/libsvgparse.so LinkedListAPI.o SVGParser.o -lxml2 

LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h
    $(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c

SVGParser.o: src/SVGParser.c include/SVGParser.h
    $(CC) $(CFLAGS) -I$(INC_PATH) -c -fpic src/SVGParser.c  -lxml2 

main: src/main.c
    $(CC) $(CFLAGS) -I$(INC_PATH) src/main.c -L./bin -lsvgparse -o main -lxml2

clean: 
    rm main -rf *.o ./bin/*.so

memTest: all
    valgrind --leak-check=full ./main