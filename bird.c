#include <curses.h>
#include <math.h>

#include "bird.h"

#define BIRD_REPR "(OvO)"

char *get_bird_repr(void) {
	    return BIRD_REPR;
}

void print_bird(struct bird *b) {
	mvprintw(b->y, b->x, get_bird_repr());
	refresh();
}

void move_bird(struct bird *b) {
	b->y += (int)round(b->acc);
}

int get_bird_size(void) {
	int i = 0;
	char *br = BIRD_REPR;
	while(br[i++] != 0x00); 
	int size = i - 1;
	return size;	
}

