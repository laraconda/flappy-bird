#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "externs.h"
#include "game.h"
#include "keys.h"

#define FFLAPPY_SRC "src/res/title.txt"
#define INST_SRC "src/res/instructions.txt"
#define CREDITS_SRC "src/res/credits.txt"

WINDOW *wscore = NULL;

void print_file(char *filename, unsigned int x, unsigned int y) {
	FILE *fp;
	if ((fp = fopen(filename, "r")) != NULL) {
		char final_str[count_chars(fp)];
		size_t i = 0;
		int c;
		for (c = getc(fp); c != EOF; c = getc(fp))
			final_str[i++] = c;
		mvprintw(y, x, final_str);
		fclose(fp);
	} else {
		fprintf(stderr, "cant open file: %s", filename);
		endwin();
		exit(1);
	}
}

int count_chars(FILE *fp) {
	unsigned int chars = 0;
	int c;
	for (c = getc(fp); c != EOF; c = getc(fp))
		chars++;
	if (fseek(fp, 0L, SEEK_SET) != 0) {
		fprintf(stderr, "Repositioning error.");
		endwin();
		exit(1);
	}
	return chars;
}

void print_title_screen() {
	print_file(FFLAPPY_SRC, 0, 0);
	print_file(INST_SRC, 0, 20);
	print_file(CREDITS_SRC, 0, 30);
}

unsigned char do_you_want_to_play(void) {
	while(1) {
	char ch = getch();
		if (ch == SPACEBAR)
			return 1;
		else if (ch == ESC)
			return 0;
	usleep(100000);
	}
}

void config_ncurses(void) {
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
}

int main (void) {
	config_ncurses();

	print_title_screen();
	refresh();

	// wait for the player to press start
	// or press ESC to quit
	if (do_you_want_to_play()) {
		clear();
		start_game();
	}
	endwin();
}
