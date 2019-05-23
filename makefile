CC=g++
CFLAGS=-I.
DEPS = linuxMSTPFunctions.h
OBJ = main.o linuxMSTPFunctions.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

serialMonitor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)    