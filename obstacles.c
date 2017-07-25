#include <string.h>
#include <curses.h>
#include <stdlib.h>

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

unsigned int n_obstacles;
struct pair_of_obstacles* pairs;

void init_obstacle(unsigned int i) {
	struct obstacle obs_a = {OBSTACLE_WIDTH, 0};
	struct obstacle obs_b = {OBSTACLE_WIDTH, LINES - 1};
	pairs[i].x = COLS + (OBSTACLE_WIDTH + OBSTACLE_SPACING) * i * 1;
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
	pairs[i].x = pairs[(i - 1 + n_obstacles) % n_obstacles].x +
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
	return (obs.x == 0);
}

void advance_obstacles(void) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++) {
		if (has_obstacle_finnished_its_track(pairs[i])) {
			insert_obstacle_behind_its_next(i);
		} else
			pairs[i].x += -1;
	}
}


void print_obstacle(unsigned int i) {
	char * obs_repr = get_obstacle_repr();
	mvprintw(pairs[i].obs_a.y, pairs[i].x, obs_repr);
	mvprintw(pairs[i].obs_b.y, pairs[i].x, obs_repr);

	free(obs_repr);
	refresh();
}

void print_obstacles(void) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++)
		print_obstacle(i);
}

void free_obstacles(void) {
	free(pairs);
}
