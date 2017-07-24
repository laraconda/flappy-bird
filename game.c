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

// simplified version of an obstacle.
struct obstacle {
	unsigned char width;
	unsigned char y;
};

struct pair_of_obstacles {
	unsigned char x;
	struct obstacle obs_a;
	struct obstacle obs_b;

};

unsigned char alive = 1;
struct pair_of_obstacles pair;

void init_obstacles(void) {
	struct obstacle obs_a = {OBSTACLE_WIDTH, 0};
	struct obstacle obs_b = {OBSTACLE_WIDTH, LINES - 1};
	pair.x = 0;
	pair.obs_a = obs_a;
	pair.obs_b = obs_b;
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

void advance_pair_of_obstacles(void) {
	pair.x += 1;
}

void periodic_events(void) {
	advance_pair_of_obstacles();
}

void *listen_controller(void * arg) {
	while(alive) {
		if (getch() == SPACEBAR) {
			pair.obs_a.y += 1;
			//pair.obs_b.y += 1;
		}
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

void print_screen(void) {	
	clear();

	char * obs_repr = get_obstacle_repr();
	mvprintw(pair.obs_a.y, pair.x, obs_repr);
	mvprintw(pair.obs_b.y, pair.x, obs_repr);
	
	free(obs_repr);
	refresh();
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
}
