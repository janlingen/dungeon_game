CFLAGS := -std=c99 -g -Wall -Wmissing-prototypes -Werror
SOURCES := dungeon.c
HEADERS := dungeon.h

run: $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) $(SOURCES) -o dungeon
	./dungeon
