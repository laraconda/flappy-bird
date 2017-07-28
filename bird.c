#include <curses.h>
#include <math.h>

#include "bird.h"

char *get_bird_repr(void) {
	    return ">>>";
}

void print_bird(struct bird *b) {
	mvprintw(b->y, b->x, get_bird_repr());
	refresh();
}

void move_bird(struct bird *b) {
	b->y += (int)round(b->acc);
 }


