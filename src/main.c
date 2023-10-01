#include <curses.h>
#include <stdlib.h>

#include "welcome.h"
#include "settings.h"

/*
 * Sets up ncurses.
 */
void config_ncurses(void) {
	initscr();
    start_color();
    use_default_colors();
    init_pair(COLOR_RED_D, COLOR_RED, -1);
    init_pair(COLOR_CYAN_D, COLOR_CYAN, -1);
    init_pair(COLOR_YELLOW_D, COLOR_YELLOW, -1);
    init_pair(COLOR_GREEN_D, COLOR_GREEN, -1);
    init_pair(COLOR_BLACK_D, COLOR_BLACK, -1);
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
