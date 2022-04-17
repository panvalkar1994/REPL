CC=gcc -Wall
SRC=src/*.c
TARGET=build/main

all:
	$(CC) $(SRC) -o $(TARGET)

clean:
	rm $(TARGET)