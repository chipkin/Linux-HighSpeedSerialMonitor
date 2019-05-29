CC=g++
CFLAGS=-I.
DEPS = linuxMSTPFunctions.h
OBJ = main.o linuxMSTPFunctions.o 
NAME := serialMonitor

.DEFAULT_GOAL := all

all: $(NAME)

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

$(NAME) : $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:	
	rm -f $(OBJ) $(NAME) log.txt 
