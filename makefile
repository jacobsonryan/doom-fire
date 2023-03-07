WARNS = -Wall -ansi -pedantic -std=c99
LEVEL = -O0
LINUX = -lSDL2
ENTRY = main.c

all:
	gcc $(ENTRY) $(LEVEL) $(WARNS) $(LINUX) -lm -o  main && ./main
