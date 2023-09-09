#include <curses.h>
#include <stdio.h>
#include <stdlib.h>

#include "externs.h"
#include "game.h"
#include "keys.h"
#include "helpers.h"


void print_title() {
    char* title =
        "  ,---.,--.                               \n"
        " /  .-'|  | ,--,--. ,---.  ,---.,--. ,--. \n"
        " |  `-,|  |' ,-.  || .-. || .-. |\\  '  /  \n"
        " |  .-'|  |\\ '-'  || '-' '| '-' ' \\   '   \n"
        " `--'  `--' `--`--'|  |-' |  |-'.-'  /    \n"
        "                   `--'   `--'  `---'     \n"
        "                                          \n"
        "      ,--.   ,--.          ,--.           \n"
        "      |  |-. `--',--.--. ,-|  |           \n"
        "      | .-. ',--.|  .--'' .-. |           \n"
        "      | `-' ||  ||  |   \\ `-' |           \n"
        "       `---' `--'`--'    `---'            \n"
        "                                          \n"
        "                                          \n"
        "       PRESS SPACEBAR TO JUMP/START       \n"
        "             PRESS ESC TO EXIT            \n";


    print_string_middle_screen(title);
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

	print_title();
	refresh();

	// wait for the player to press start
	// or press ESC to quit
	if (do_you_want_to_play()) {
		clear();
		game_loop();
	}
	endwin();
}
