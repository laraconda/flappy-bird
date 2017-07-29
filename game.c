#include <pthread.h>
#include <curses.h>
#include <stdlib.h>

#include "externs.h"
#include "bird.h"
#include "obstacles.h"
#include "keys.h"
#include "nwindows.h"

#define FPS 15 // frames per second
#define SECOND_IN_MICROSECONDS 1000000
#define SPEED SECOND_IN_MICROSECONDS / FPS

#define OBSTACLE_WIDTH 6
#define OBSTACLE_SPACING 20
#define MIN_VERTICAL_GAP 20
#define MAX_Y_DIFF_BETWEEN_NEIGHBORS 15

#define MAX_ACC 4.0


struct WIN_pos_size STD_WIN;


pthread_mutex_t bird_acc_mutex;

unsigned char alive = 1;
unsigned long score = 0;

// Initial bird position and acceleration
struct bird b = {10, 2, 1.0}; 

struct obstacles_settings obs_sett;
struct pair_of_obstacles *pairs;
unsigned int n_obstacles;

void accelerate_bird(void) {
	if (b.acc < MAX_ACC) {
		pthread_mutex_lock(&bird_acc_mutex);
		b.acc += 0.6; 
		pthread_mutex_unlock(&bird_acc_mutex);
	}
}

char* get_score_message(void) {
	char *score_s_template = "SCORE: %lu";
	int buff_size = snprintf(NULL, 0, score_s_template, score);
	char *score_s = malloc(buff_size + 1);
	snprintf(score_s, buff_size + 1, score_s_template, score);
	return score_s;
}

void update_score_window(void) {
	wclear(wscore);
	char *score_message = get_score_message();
	mvwprintw(wscore, 0, 0, score_message);
	wrefresh(wscore);
	free(score_message);
}

unsigned char is_there_any_obstacle_at_x(int x) {
	unsigned int i;     
	for (i=0; i<n_obstacles; i++)
		if (pairs[i].x == x)
			return 1;
    return 0;
}

void refresh_score(void) {
	if (is_there_any_obstacle_at_x(b.x)) {
		score += 1;
		update_score_window();
	}
}

// This only works for birds that are 1 line in height
// and when the obstacles are rectangular.
// Its easy to change it to support k lines in height
// but it would only work if the bird is a rectangle.
unsigned char is_bird_crashing_against_obstacles(void) {
	int xobs1, x1 = b.x + get_bird_size();
	unsigned int i;
	int y0, y1;
	for (i = 0; i<n_obstacles; i++) {
		xobs1 = pairs[i].x + obs_sett.width;
		y0 = pairs[i].obs_a.y;
		y1 = pairs[i].obs_b.y;
		if (((x1 >= pairs[i].x && b.x <= pairs[i].x) ||
			(b.x <= xobs1 && x1 >= xobs1)) &&
			((b.y >= y0 && b.y >= y1) || (b.y <= y1 && b.y <= y0))
		)
			return 1;
	}
	return 0;
}

unsigned char is_bird_outside_bounds(void) {
	// Not checking x because x its fixed.
	return (b.y < 0 || b.y >= STD_WIN.height);
}

void check_dead(void) {
	if (is_bird_outside_bounds() ||
		is_bird_crashing_against_obstacles())
		alive = 0;
}

void periodic_events(void) {
	accelerate_bird();
	advance_obstacles(STD_WIN, pairs, n_obstacles, obs_sett);
	move_bird(&b);
	refresh_score();
	check_dead();
}

void *listen_controller(void * arg) {
	while(alive) {
		char ch = getch();
		if (ch == SPACEBAR) {
			pthread_mutex_lock(&bird_acc_mutex);
			b.acc = -3.0; 
			pthread_mutex_unlock(&bird_acc_mutex);
		} else if (ch == ESC) 
			alive = 0;
	}

	pthread_exit(NULL);
}

void init_controller_listener(void) {
	int rc;
	pthread_t tid;
	pthread_mutex_init(&bird_acc_mutex, NULL);
	rc = pthread_create(&tid, NULL, &listen_controller, NULL);
	if (rc) {
		fprintf(stderr, "cant create new thread! error code %d", rc);
		exit(1);
	}
}

void print_screen(void) {	
	clear();
	print_obstacles(STD_WIN, pairs, n_obstacles, obs_sett);
	print_bird(&b);
}

void print_score_message(void) {
	char *score_message = get_score_message();
	mvprintw(
		(STD_WIN.height/2) + 1, STD_WIN.width/2, score_message);
	refresh();
	free(score_message);
}

void print_death_message(void) {
	clear();
	mvprintw(
		STD_WIN.height/2, STD_WIN.width/2, "YOU JUST DIED BOI");
	print_score_message();	
	refresh();
	sleep(2);
}

void update_STD_WIN(
	int startx, int starty, int height, int width) {
	STD_WIN.startx = startx;
	STD_WIN.starty = starty;
	STD_WIN.width = width;
	STD_WIN.height = height;
}

void set_up_windows(void) {
	wresize(stdscr, LINES - 1, COLS);
	update_STD_WIN(0, 0, LINES - 1, COLS);
	wscore = newwin(1, COLS, LINES - 1, 0);
}

void fill_obstacle_settings(void) {
	obs_sett.width = OBSTACLE_WIDTH;
	obs_sett.spacing = OBSTACLE_SPACING;
	obs_sett.min_vert_gap = MIN_VERTICAL_GAP;
	obs_sett.max_vert_diff_neighbors =
		MAX_Y_DIFF_BETWEEN_NEIGHBORS;
}

void set_up_obstacles(void) {
	fill_obstacle_settings();

	n_obstacles = (
		STD_WIN.width / (obs_sett.width + obs_sett.spacing)) + 1;

	pairs = malloc(n_obstacles * sizeof(struct pair_of_obstacles));
	init_obstacles(STD_WIN, pairs, n_obstacles, obs_sett);
}

void clean(void) {
	pthread_mutex_destroy(&bird_acc_mutex);
	free(pairs);
}

void start_game(void) {
	set_up_windows();
	update_score_window();

	set_up_obstacles();
	init_controller_listener();
	while(alive) {
		
		print_screen();
		periodic_events();
		
		usleep(SPEED);
	}
	clean();	
	print_death_message();
}
