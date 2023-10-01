#include <curses.h>
#include <stdlib.h>

#include "externs.h"
#include "bird.h"
#include "obstacles.h"
#include "keys.h"
#include "nwindows.h"
#include "helpers.h"
#include "game.h"
#include "settings.h"

#define SECOND_IN_MICROSECONDS 1000000
#define SPEED SECOND_IN_MICROSECONDS / FPS


WINDOW *score_window;

struct WIN_pos_size STD_WIN;


bool alive;
unsigned long score;

struct bird bird;

struct obstacles_settings obs_sett;
struct pair_of_obstacles *obstacle_pairs;
unsigned int n_obstacles;

/*
 * Accelerates the bird towards the floor.
 */
void accelerate_bird(void) {
	if (bird.acc < MAX_ACC) {
		bird.acc += 0.6; 
        bird.acc = bird.acc > MAX_ACC ? MAX_ACC : bird.acc;
	}
}

/*
 * Returns the string that comunicates the score
 */
char* get_score_message(void) {
	char *score_s_template = "SCORE: %lu";
	int buff_size = snprintf(NULL, 0, score_s_template, score);
	char *score_s = malloc(buff_size + 1);
	snprintf(score_s, buff_size + 1, score_s_template, score);
	return score_s;
}

/*
 * Updates the score window with the current score.
 */
void update_score_window(void) {
	wclear(score_window);
	char *score_message = get_score_message();
	mvwprintw(score_window, 0, 0, score_message);
	wrefresh(score_window);
	free(score_message);
}

/*
 * Returns 1 if an obstacle is at coordinate x.
 * 0 otherwise.
 */
unsigned char is_there_an_obstacle_at(int x) {
	unsigned int i;     
	for (i=0; i<n_obstacles; i++)
		if (obstacle_pairs[i].x == x)
			return 1;
    return 0;
}

/*
 * Checks if the bird has cleared a new obstacle.
 * Updates the score window if necessary.
 */
void refresh_score(void) {
	if (is_there_an_obstacle_at(bird.x)) {
		score += 1;
		update_score_window();
	}
}

/*
 * Checks if the bird is crashing against an obstacle.
 *
 * This only works for birds that are 1 line in height
 * and when the obstacles are rectangular.
 * Its easy to change it to support k lines in height
 * but it would only work if the bird is a rectangle.
 */
unsigned char is_bird_crashing_against_obstacles(void) {
	int xobs1, x1 = bird.x + get_bird_size();
	unsigned int i;
	int y0, y1;
	for (i = 0; i<n_obstacles; i++) {
		xobs1 = obstacle_pairs[i].x + obs_sett.width;
		y0 = obstacle_pairs[i].top_obstacle.y;
		y1 = obstacle_pairs[i].bottom_obstacle.y;
		if (((x1 >= obstacle_pairs[i].x && bird.x <= obstacle_pairs[i].x) ||
			(bird.x <= xobs1 && x1 >= xobs1)) &&
			((bird.y >= y0 && bird.y >= y1) || (bird.y <= y1 && bird.y <= y0))
		)
			return 1;
	}
	return 0;
}

/*
 * Checks if the bird is outside of the bounds
 * of the game window.
 */
unsigned char is_bird_outside_bounds(void) {
	// Not checking x because x its fixed.
	return (bird.y < 0 || bird.y >= STD_WIN.height);
}

/*
 * If the bird is outside of the game window
 * or has crashed against an obstacle it is marked as dead.
 */
void check_dead(void) {
	if (
        is_bird_outside_bounds() ||
		is_bird_crashing_against_obstacles()
    ) {
		alive = false;
    }
}

/*
 * Clears the screen and prints a frame of the game.
 */
void print_game_frame(void) {	
	clear();
	print_obstacles(STD_WIN, obstacle_pairs, n_obstacles, obs_sett);
	print_bird(&bird);
}

void listen_jump_controller() {
    char ch = getch();
    if (ch == SPACEBAR) {
        bird.acc = -3.0; 
    } else if (ch == ESC) {
        alive = 0;
    }
}

/*
 * Performs the periodic events the game needs to run.
 */
void periodic_events(void) {
    print_game_frame();
    listen_jump_controller();
	accelerate_bird();
	advance_obstacles(STD_WIN, obstacle_pairs, n_obstacles, obs_sett);
	move_bird(&bird);
	refresh_score();
	check_dead();
}

/*void print_score_message(void) {
	char *score_message = get_score_message();
	mvprintw(
		(STD_WIN.height/2) + 1, STD_WIN.width/2, score_message);
	refresh();
	free(score_message);
}*/

/*
 * Waits for the user to decide to try again or exit the program.
 */
bool input_try_again() {
    char ch = getch();
    if (ch == SPACEBAR) {
        return true;
    } else if (ch == ESC) {
        return false;
    } else if (ch == ERR) {
        return false; 
    }
}

/*
 * After the user dies, this screen is shown.
 */
void show_death_screen(void) {
    wclear(score_window);
    wrefresh(score_window);

    attron(A_BOLD);
    attron(COLOR_PAIR(COLOR_BLACK_D));
    char* death_message =
        "     YOU JUST DIED BOI      \n"
        "                            \n"
        "                            \n"
        "PRESS SPACEBAR TO PLAY AGAIN\n"
        "     PRESS ESC TO EXIT      \n";
    print_string_middle_screen(death_message);
    attroff(COLOR_PAIR(COLOR_BLACK_D));
    attroff(A_BOLD);
	
    // print_score_message();	
	refresh();
}


void update_STD_WIN(
	int startx, int starty, int height, int width) {
	STD_WIN.startx = startx;
	STD_WIN.starty = starty;
	STD_WIN.width = width;
	STD_WIN.height = height;
}

/*
 * Sets up the windows for the game: the main window and the score
 * window.
 */
void set_up_windows(void) {
	wresize(stdscr, LINES - 1, COLS);
	update_STD_WIN(0, 0, LINES - 1, COLS);
	score_window = newwin(1, COLS, LINES - 1, 0);
    wclear(score_window);
    wrefresh(score_window);
}

/* 
 * Creates a struct containing the settings for the obstacles.
 */
void fill_obstacle_settings(void) {
	obs_sett.width = OBSTACLE_WIDTH;
	obs_sett.spacing = OBSTACLE_SPACING;
	obs_sett.min_vert_gap = MIN_VERTICAL_GAP;
	obs_sett.max_vert_diff_neighbors =
		MAX_Y_DIFF_BETWEEN_NEIGHBORS;
}

/*
 * Allocates memory for the obstacles and initializes them.
 */
void set_up_obstacles(void) {
	fill_obstacle_settings();
	n_obstacles = (
		STD_WIN.width / (obs_sett.width + obs_sett.spacing)
    ) + 1;
	obstacle_pairs = malloc(n_obstacles * sizeof(struct pair_of_obstacles));
	init_obstacles(STD_WIN, obstacle_pairs, n_obstacles, obs_sett);
}

/*
 * Destroys the mutex for the bird.
 * Frees the memory of the obstacles.
 */
void clean_after_game(void) {
    clear();
	free(obstacle_pairs);
}

/*
 * Initializes the conditions for a fresh game.
 */
void init_game_state() {
    alive = true;
    score = 0;
    // Initial bird position (y, x) and acceleration
    bird = (struct bird){10, 2, 1.0}; 
	set_up_obstacles();
}

/*
 * Game loop. Frames and game events are printed/calculated
 * periodically.
 */
void game_loop(void) {
    bool play = true;
    while (play) {
        set_up_windows();
        
        init_game_state();
        update_score_window();
        while(alive) {
            periodic_events();
            usleep(SPEED);
        }
        clean_after_game();	
        show_death_screen();
        play = input_try_again();
    }
}
