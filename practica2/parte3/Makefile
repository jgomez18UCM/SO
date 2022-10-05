CC = gcc
CFLAGS = -Wall -g

PROG = binwrite
OBJECTS = $(PROG).o
LIBS = defs.h

all: $(PROG)

$(PROG): $(OBJECTS)
		$(CC) -g -o $(PROG) $(OBJECTS) $(LIBS)

%.o : %.c
		$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean

clean: 
	rm $(PROG) $(OBJECTS)