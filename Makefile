CC=cc
CFLAGS=-Wall -Werror
LIBS=-llua -lSDL2 -lSDL2_gfx -Isrc
SOURCES=$(shell find src -name "*.c")
INCLUDES=$(shell find src -name "*.h")

glua: $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) -o glua $(SOURCES) $(LIBS)

