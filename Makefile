CC=cc
CFLAGS=-Wall -Werror -Wextra -pedantic
LIBS=-llua -lSDL2 -lSDL2_gfx -lSDL2_mixer -lSDL2_image -Isrc
SOURCES=$(shell find src -name "*.c")
INCLUDES=$(shell find src -name "*.h")

glua: $(SOURCES) $(INCLUDES)
	$(CC) $(CFLAGS) -o glua $(SOURCES) $(LIBS)

