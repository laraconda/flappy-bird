#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define OBSTACLE_WIDTH 6
#define OBSTACLE_SPACING 20

#define MIN_VERTICAL_GAP 15
#define MAX_Y_DIFF_BETWEEN_NEIGHBORS 15


// simplified version of an obstacle.
struct obstacle {
	char width;
	char y;
	char height;
};

struct pair_of_obstacles {
	char x;
	struct obstacle obs_a;
	struct obstacle obs_b;

};

unsigned int n_obstacles;
struct pair_of_obstacles* pairs;

void init_obstacle(unsigned int i, char y0, char y1) {
	struct obstacle obs_a = {OBSTACLE_WIDTH, y0, y0 + 1};
	struct obstacle obs_b = {OBSTACLE_WIDTH, y1, LINES - y1};
	pairs[i].x = COLS + (OBSTACLE_WIDTH + OBSTACLE_SPACING) * i * 1;
	pairs[i].obs_a = obs_a;
	pairs[i].obs_b = obs_b;
}

void generate_obstacles_y_positions(
	char* last_y0, char* last_y1, unsigned int *seed) {
	if (*last_y0 == -1 || *last_y1 == -1)
		*last_y0 = rand_r(seed) % ((LINES - 1) - MIN_VERTICAL_GAP);
	else {
		unsigned int diff = rand_r(seed) % MAX_Y_DIFF_BETWEEN_NEIGHBORS;
		if (*last_y0 < LINES/2)
			*last_y0 = *last_y0 + diff;
		else
			*last_y0 = *last_y0 - diff;
	}
	if (*last_y0 + MIN_VERTICAL_GAP > LINES - 1)
		*last_y0 = (LINES - 1) - MIN_VERTICAL_GAP;
	*last_y1 = *last_y0 + MIN_VERTICAL_GAP;
}

void init_obstacles(void) {
	srand(time(NULL));
	unsigned int i;
	n_obstacles = (COLS/ (OBSTACLE_WIDTH + OBSTACLE_SPACING)) + 1;
	pairs = malloc(
			n_obstacles * sizeof(struct pair_of_obstacles));

	char last_y0[1] = {-1};
	char last_y1[1] = {-1};
	unsigned int seed = rand();
	for (i=0; i<n_obstacles; i++) {
		generate_obstacles_y_positions(last_y0, last_y1, &seed);
		init_obstacle(i, *last_y0, *last_y1);
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


void print_obstacle(unsigned int i) {
	if (pairs[i].x >= 0 && pairs[i].x <= COLS - OBSTACLE_WIDTH) {
		char * block = get_obstacle_block();
		size_t j, k;
		for (j = 0; j <= pairs[i].obs_a.y; j++)
			mvprintw(j, pairs[i].x, block);
	
		for (k = pairs[i].obs_b.y; k < LINES; k++) 
			mvprintw(k, pairs[i].x, block);

		free(block);
		refresh();
	}
}

void print_obstacles(void) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++)
		print_obstacle(i);
}

void free_obstacles(void) {
	free(pairs);
}
