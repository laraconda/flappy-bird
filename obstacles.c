#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "nwindows.h"
#include "obstacles.h"


void init_obstacle(
	unsigned int i, char y0, char y1, int win_width,
	struct pair_of_obstacles *pairs, struct obstacles_settings sett) {
	struct obstacle obs_a = {sett.width, y0};
	struct obstacle obs_b = {sett.width, y1};
	pairs[i].x =
		win_width + (sett.width + sett.spacing) * i * 1;
	pairs[i].obs_a = obs_a;
	pairs[i].obs_b = obs_b;
}

void generate_obstacles_y_positions(
	int* lasty0, int* lasty1, unsigned int *seed,
	struct WIN_pos_size win, struct obstacles_settings sett) {
	if (*lasty0 == -1 || *lasty1 == -1)
		*lasty0 = rand_r(seed) % ((win.height-1) - sett.min_vert_gap);
	else {
		unsigned int diff = rand_r(seed) % 
			sett.max_vert_diff_neighbors;
		if (*lasty0 < win.height / 2)
			*lasty0 = *lasty0 + diff;
		else
			*lasty0 = *lasty0 - diff;
	}
	if (*lasty0 + sett.min_vert_gap > win.height - 1)
		*lasty0 = (win.height - 1) - sett.min_vert_gap;
	*lasty1 = *lasty0 + sett.min_vert_gap;
}


void init_obstacles(
	struct WIN_pos_size win, struct pair_of_obstacles *pairs,
	unsigned int n_obstacles, struct obstacles_settings sett) {
	
	srand(time(NULL));
	unsigned int i;

	int lasty0[1] = {-1};
	int lasty1[1] = {-1};
	unsigned int seed = rand();
	for (i=0; i<n_obstacles; i++) {
		generate_obstacles_y_positions(
			lasty0, lasty1, &seed, win, sett);
		init_obstacle(i, *lasty0, *lasty1, win.width, pairs, sett);
	}
}

void insert_obstacle_behind_its_next(
	unsigned int i, struct pair_of_obstacles *pairs,
	unsigned int n_obstacles, struct obstacles_settings sett) {
	pairs[i].x = pairs[(i - 1 + n_obstacles) % n_obstacles].x +
		(sett.spacing + sett.width);
}

char * get_obstacle_block(unsigned int width) {
	char *cell = "@";
	char s_local[width];
	int i;
	for (i=0; i<width; i++)
		s_local[i] = *cell;
	char *s = malloc(width * sizeof(char));
	return strcpy(s, s_local);
}

unsigned char has_obstacle_finnished_its_track(
		struct pair_of_obstacles obs) {
	return (obs.x == 0);
}

void advance_obstacles(
		struct pair_of_obstacles *pairs, unsigned int n_obstacles,
		struct obstacles_settings sett) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++) {
		if (has_obstacle_finnished_its_track(pairs[i])) {
			insert_obstacle_behind_its_next(i, pairs, n_obstacles, sett);
		} else
			pairs[i].x += -1;
	}
}

void print_obstacle(
	struct WIN_pos_size win, struct pair_of_obstacles *pairs,
	unsigned int i, struct obstacles_settings sett) {
	if (pairs[i].x >= 0 &&
		pairs[i].x <= win.width - sett.width) {
		char *block = get_obstacle_block(sett.width);
		size_t j, k;
		for (j = 0; j <= pairs[i].obs_a.y; j++)
			mvprintw(j, pairs[i].x, block);
	
		for (k = pairs[i].obs_b.y; k < win.height; k++) 
			mvprintw(k, pairs[i].x, block);

		free(block);
		refresh();
	}
}

void print_obstacles(
	struct WIN_pos_size win, struct pair_of_obstacles *pairs,
	unsigned int n_obstacles, struct obstacles_settings sett) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++)
		print_obstacle(win, pairs, i, sett);
}

