CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
EXEC = hex
LDFLAGS = -g

all: $(EXEC)

test: $(EXEC).o test.o
	$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC).o: $(EXEC).c $(EXEC).h
	$(CC) -c $< $(CFLAGS) -o $@

test.o: test.c
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
