CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
EXEC = tools
LDFLAGS = -g

all: $(EXEC)

$(EXEC): $(EXEC).o
	$(CC) -o $@ $^ $(LDFLAGS)

$(EXEC).o: $(EXEC).c $(EXEC).h
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -rf *.o
