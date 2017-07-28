#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "nwindows.h"

#define OBSTACLE_WIDTH 6
#define OBSTACLE_SPACING 20

#define MIN_VERTICAL_GAP 15
#define MAX_Y_DIFF_BETWEEN_NEIGHBORS 15


struct obstacle {
	char width;
	int y;
};

struct pair_of_obstacles {
	int x;
	struct obstacle obs_a;
	struct obstacle obs_b;

};

unsigned int n_obstacles;
struct pair_of_obstacles* pairs = NULL;

void init_obstacle(
	unsigned int i, char y0, char y1, int win_width) {
	struct obstacle obs_a = {OBSTACLE_WIDTH, y0};
	struct obstacle obs_b = {OBSTACLE_WIDTH, y1};
	pairs[i].x =
		win_width + (OBSTACLE_WIDTH + OBSTACLE_SPACING) * i * 1;
	pairs[i].obs_a = obs_a;
	pairs[i].obs_b = obs_b;
}

void generate_obstacles_y_positions(
	int* lasty0, int* lasty1, unsigned int *seed,
	struct WIN_pos_size win) {
	if (*lasty0 == -1 || *lasty1 == -1)
		*lasty0 = rand_r(seed) % ((win.height - 1) - MIN_VERTICAL_GAP);
	else {
		unsigned int diff = rand_r(seed) % 
			MAX_Y_DIFF_BETWEEN_NEIGHBORS;
		if (*lasty0 < win.height / 2)
			*lasty0 = *lasty0 + diff;
		else
			*lasty0 = *lasty0 - diff;
	}
	if (*lasty0 + MIN_VERTICAL_GAP > LINES - 1)
		*lasty0 = (win.height - 1) - MIN_VERTICAL_GAP;
	*lasty1 = *lasty0 + MIN_VERTICAL_GAP;
}

void init_obstacles(struct WIN_pos_size win) {
	srand(time(NULL));
	unsigned int i;
	n_obstacles =
		(COLS / (OBSTACLE_WIDTH + OBSTACLE_SPACING)) + 1;
	pairs = malloc(
		n_obstacles * sizeof(struct pair_of_obstacles));

	int lasty0[1] = {-1};
	int lasty1[1] = {-1};
	unsigned int seed = rand();
	for (i=0; i<n_obstacles; i++) {
		generate_obstacles_y_positions(lasty0, lasty1, &seed, win);
		init_obstacle(i, *lasty0, *lasty1, win.width);
	}
}

void insert_obstacle_behind_its_next(unsigned int i) {
	pairs[i].x = pairs[(i - 1 + n_obstacles) % n_obstacles].x +
		(OBSTACLE_SPACING + OBSTACLE_WIDTH);
}

char * get_obstacle_block() {
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

void print_obstacle(unsigned int i, struct WIN_pos_size win) {
	if (pairs[i].x >= 0 &&
		pairs[i].x <= win.width - OBSTACLE_WIDTH) {
		char * block = get_obstacle_block();
		size_t j, k;
		for (j = 0; j <= pairs[i].obs_a.y; j++)
			mvprintw(j, pairs[i].x, block);
	
		for (k = pairs[i].obs_b.y; k < win.height; k++) 
			mvprintw(k, pairs[i].x, block);

		free(block);
		refresh();
	}
}

void print_obstacles(struct WIN_pos_size win) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++)
		print_obstacle(i, win);
}

void free_obstacles(void) {
	free(pairs);
}

unsigned char is_there_any_obstacle_at_x(int x) {
	unsigned int i; 	
	for (i=0; i<n_obstacles; i++)
		if (pairs[i].x == x)
			return 1;
	return 0;
}
