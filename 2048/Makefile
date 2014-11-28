CC	   ?= gcc
CFLAGS += -Wall -Wextra
LIBS	= -lcurses

all: 2048

2048: 2048.c
	$(CC) $(CFLAGS) 2048.c hiScoreHandler.c -o 2048 $(LIBS)
	
clean:
	rm -f 2048
