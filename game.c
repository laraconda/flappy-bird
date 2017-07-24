#include <pthread.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "keys.h"

#define FPS 15 // frames per second
#define SECOND_IN_MICROSECONDS 1000000
#define SPEED SECOND_IN_MICROSECONDS / FPS

#define OBSTACLE_WIDTH 4
#define OBSTACLE_SPACING 5

// simplified version of an obstacle.
struct obstacle {
	char width;
	char y;
};

struct pair_of_obstacles {
	char x;
	struct obstacle obs_a;
	struct obstacle obs_b;

};

unsigned char alive = 1;
unsigned int n_obstacles;
struct pair_of_obstacles* pairs;

void init_obstacle(unsigned int i) {
	struct obstacle obs_a = {OBSTACLE_WIDTH, 0};
	struct obstacle obs_b = {OBSTACLE_WIDTH, LINES - 1};
	pairs[i].x = (OBSTACLE_WIDTH + OBSTACLE_SPACING) * i * (-1);
	pairs[i].obs_a = obs_a;
	pairs[i].obs_b = obs_b;
}

void init_obstacles(void) {
	unsigned int i;
	n_obstacles = (COLS/ (OBSTACLE_WIDTH + OBSTACLE_SPACING)) + 1;
	pairs = malloc(
		n_obstacles * sizeof(struct pair_of_obstacles));
	for (i=0; i<n_obstacles; i++) {
		init_obstacle(i);
	}
}

void insert_obstacle_behind_its_next(unsigned int i) {
	pairs[i].x = pairs[(i - 1 + n_obstacles) % n_obstacles].x -
		(OBSTACLE_SPACING + OBSTACLE_WIDTH);
}

char * get_obstacle_repr(void) {
	char *cell = "@";
	char s_local[OBSTACLE_WIDTH];
	int i;
	for (i=0; i<OBSTACLE_WIDTH; i++)
		s_local[i] = *cell;
	char *s = malloc(OBSTACLE_WIDTH * sizeof(char));
	return strcpy(s, s_local);
}

unsigned char has_obstacle_finnished_its_track(
	struct pair_of_obstacles obs) {
	return (obs.x >= COLS);
}

void advance_obstacles(void) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++) {
		if (has_obstacle_finnished_its_track(pairs[i])) {
			insert_obstacle_behind_its_next(i);
		} else
			pairs[i].x += 1;
	}
}

void periodic_events(void) {
	advance_obstacles();
}

void *listen_controller(void * arg) {
	while(alive) {
		if (getch() == SPACEBAR)
			advance_obstacles();
	}

	pthread_exit(NULL);
}

void init_controller_listener(void) {
	int rc;
	pthread_t tid;
	rc = pthread_create(&tid, NULL, &listen_controller, NULL);
	if (rc) {
		fprintf(stderr, "cant create new thread! error code %d", rc);
		exit(1);
	}
}

void print_obstacle(unsigned int i) {
	char * obs_repr = get_obstacle_repr();
	mvprintw(pairs[i].obs_a.y, pairs[i].x, obs_repr);
	mvprintw(pairs[i].obs_b.y, pairs[i].x, obs_repr);
	
	free(obs_repr);
	refresh();
}

void print_obstacles() {
	unsigned int i;
	for (i=0; i<n_obstacles; i++)
		print_obstacle(i);
}


void print_screen(void) {	
	clear();
	print_obstacles();
}

void start_game(void) {
	init_obstacles();
	init_controller_listener();
	while(alive) {
		print_screen();
		periodic_events();
		usleep(SPEED);
	}

	pthread_exit(NULL);
	free(pairs);
}
