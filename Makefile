# Makefile
LDFLAGS=-lncurses -lpthread -lm

flappy: obstacles.o bird.o game.o welcome.o
	cc -o flappy obstacles.o bird.o game.o welcome.o $(LDFLAGS)
obstacles.o: obstacles.c obstacles.h nwindows.h
	cc -c obstacles.c
bird.o: bird.c bird.c bird.h
	cc -c bird.c
game.o: game.c externs.h bird.h obstacles.h keys.h nwindows.h
	cc -c game.c
welcome.o: welcome.c externs.h game.h keys.h
	cc -c welcome.c

clean:
	rm *.o flappy
