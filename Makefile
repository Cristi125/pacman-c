CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJ = main.o game.o map.o render.o input.o

pacman: $(OBJ)
	$(CC) $(CFLAGS) -o pacman $(OBJ)

main.o: main.c game.h render.h input.h
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h map.h
	$(CC) $(CFLAGS) -c game.c

map.o: map.c map.h game.h
	$(CC) $(CFLAGS) -c map.c

render.o: render.c render.h game.h
	$(CC) $(CFLAGS) -c render.c

input.o: input.c input.h
	$(CC) $(CFLAGS) -c input.c

clean:
	rm -f *.o pacman
