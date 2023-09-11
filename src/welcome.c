#include <curses.h>

#include "externs.h"
#include "game.h"
#include "keys.h"
#include "helpers.h"
#include "welcome.h"


void print_title(void) {
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
 * Presents the welcome screen.
 */
void welcome_screen(void) {
	print_title();
	if (input_start_game()) {
		clear();
		game_loop();
	}
}
