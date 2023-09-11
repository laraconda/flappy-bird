#include <curses.h>

#include "welcome.h"

/*
 * Sets up ncurses.
 */
void config_ncurses(void) {
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
}

/*
 * Terminates ncourses.
 */
void clean_before_exit() {
    endwin();
}

int main (void) {
	config_ncurses();
    welcome_screen();
    clean_before_exit();
    return 0;
}
