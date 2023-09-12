#include <curses.h>
#include <math.h>

#include "bird.h"
#include "settings.h"


char* get_bird_repr(void) {
    return BIRD_REPR;
}

void print_bird(struct bird *b) {
    attron(COLOR_PAIR(COLOR_YELLOW_D));
	mvprintw(b->y, b->x, get_bird_repr());
    attroff(COLOR_PAIR(COLOR_YELLOW_D));
	refresh();
}

void move_bird(struct bird *b) {
	b->y += (int)round(b->acc);
}

int get_bird_size(void) {
	int i = 0;
	char *br = BIRD_REPR;
	while(br[i++] != 0x00); 
	return i - 1;
}

