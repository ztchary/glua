CC=cc
CFLAGS=-Wall -Werror
LIBS=-llua -lSDL2

glua: glua.c
	$(CC) $(CFLAGS) -o glua glua.c $(LIBS)

