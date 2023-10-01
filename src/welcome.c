#include <curses.h>
#include <stdlib.h>
#include <string.h>

#include "externs.h"
#include "game.h"
#include "keys.h"
#include "helpers.h"
#include "welcome.h"
#include "settings.h"


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
        "                                          \n";
    char* instructions = 
        "      PRESS SPACEBAR TO JUMP/START        \n"
        "            PRESS ESC TO EXIT             \n";

    attron(COLOR_PAIR(COLOR_CYAN_D));
    attron(A_BOLD);
    int lasty = print_string_middle_screen(title);
    attroff(COLOR_PAIR(COLOR_CYAN_D));
    
    attron(COLOR_PAIR(COLOR_BLACK_D));
    print_string_centered_x_axis(instructions, lasty + 2);
    attroff(COLOR_PAIR(COLOR_BLACK_D));
    attroff(A_BOLD);

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
    else if (ch == ERR) {
        return true;   
    }
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
