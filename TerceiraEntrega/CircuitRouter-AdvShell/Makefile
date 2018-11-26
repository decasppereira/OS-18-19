# Makefile, versao 1
# Sistemas Operativos

SOURCES = CircuitRouter-AdvShell.c 
SOURCES+= ../lib/commandlinereader.c ../lib/vector.c
OBJS = $(SOURCES:%.c=%.o)
CC   = gcc
CFLAGS =-Wall -std=gnu99 -I../ -g
LDFLAGS=-lm
TARGET = CircuitRouter-AdvShell

all: $(TARGET) CircuitRouter-SeqSolver CircuitRouter-Client

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(TARGET) $(LDFLAGS) 

CircuitRouter-AdvShell.o: CircuitRouter-AdvShell.c CircuitRouter-AdvShell.h ../lib/commandlinereader.h ../lib/vector.h
../lib/commandlinereader.o: ../lib/commandlinereader.c ../lib/commandlinereader.h
../lib/vector.o: ../lib/vector.c ../lib/vector.h ../lib/types.h ../lib/utility.h

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $<

CircuitRouter-SeqSolver:
	(cd ../CircuitRouter-SeqSolver/ && make)

CircuitRouter-Client: CircuitRouter-Client.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)


clean:
	@echo Cleaning...
	rm -f $(OBJS) $(TARGET)
	rm -f $(OBJS) CircuitRouter-Client
	rm -f $(OBJS) CircuitRouter-Client.o
	rm -f client*
	rm -f *.pipe
	(cd ../CircuitRouter-SeqSolver/ && make clean) 
