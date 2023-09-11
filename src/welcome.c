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
	refresh();
}

/*
 * Waits for the user to decide to start the game or exit the program.
 */
bool input_start_game(void) {
    char ch = getch();
    if (ch == SPACEBAR)
        return true;
    else if (ch == ESC)
        return false;
}

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
	print_title();
	if (input_start_game()) {
		clear();
		game_loop();
	}
    clean_before_exit();
    return 0;
}
