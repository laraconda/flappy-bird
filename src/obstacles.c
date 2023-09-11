#include <string.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "nwindows.h"
#include "obstacles.h"

/*
 * Inits a single pair of obstacles.
 */
void init_obstacle(
	unsigned int i,
    char y0,
    char y1,
    int win_width,
	struct pair_of_obstacles *pairs,
    struct obstacles_settings sett
) {
	struct obstacle top_obstacle = {sett.width, y0};
	struct obstacle bottom_obstacle = {sett.width, y1};
	pairs[i].x =
		win_width + (sett.width + sett.spacing) * i * 1;
	pairs[i].top_obstacle = top_obstacle;
	pairs[i].bottom_obstacle = bottom_obstacle;
}

/* 
 * Sets the y positions of the top and bottom obstacles in a random method.
 */
void generate_obstacles_y_positions(
	int* lasty0,
    int* lasty1,
    unsigned int *seed,
	struct WIN_pos_size win,
    struct obstacles_settings sett
) {
	if (*lasty0 == -1 || *lasty1 == -1) {
		*lasty0 = rand_r(seed) % ((win.height-1) - sett.min_vert_gap);
    } else {
		unsigned int diff = rand_r(seed) %
			sett.max_vert_diff_neighbors;
		char positive = rand_r(seed) % 2;
        *lasty0 += (positive) ? diff : diff; 
	}
	if (*lasty0 + sett.min_vert_gap > win.height - 1) {
		*lasty0 = (win.height - 1) - sett.min_vert_gap;
    }
    if (*lasty0 < 0) {
		*lasty0 = 0;
    }
	*lasty1 = *lasty0 + sett.min_vert_gap;
}

/*
 * Initializes the first batch of obstacles.
 */
void init_obstacles(
	struct WIN_pos_size win,
    struct pair_of_obstacles *pairs,
	unsigned int n_obstacles,
    struct obstacles_settings sett
) {
	srand(time(NULL));
	unsigned int i;

	int lasty0[1] = {-1};
	int lasty1[1] = {-1};
	unsigned int seed = rand();
	for (i=0; i<n_obstacles; i++) {
		generate_obstacles_y_positions(
		    lasty0,
            lasty1,
            &seed,
            win,
            sett
        );
		init_obstacle(i, *lasty0, *lasty1, win.width, pairs, sett);
	}
}

/*
 * Puts an obstacle back at the end of the game track.
 * The y positions of the top and bottom obstacles are recalculated.
 */
void recycle_obstacle(
	struct WIN_pos_size win,
	unsigned int i,
    struct pair_of_obstacles *pairs,
	unsigned int n_obstacles,
    struct obstacles_settings sett
) {
	unsigned int seed = rand();
	unsigned int next_i = (i - 1 + n_obstacles) % n_obstacles;
	pairs[i].x = pairs[next_i].x + (sett.spacing + sett.width);
	int lasty0[1] = {pairs[next_i].top_obstacle.y};
	int lasty1[1] = {pairs[next_i].bottom_obstacle.y};
	generate_obstacles_y_positions(lasty0, lasty1, &seed, win, sett);
	pairs[i].top_obstacle.y = *lasty0;
	pairs[i].bottom_obstacle.y = *lasty1;
}

/*
 * Returns the representation of an obstacle.
 */
char* get_obstacle_repr(unsigned int width) {
	char *cell = "@";
	char s_local[width + 1];
	for (int i=0; i<width; i++) {
		s_local[i] = *cell;
    }
    s_local[width] = '\0';
	char *s = malloc(width * sizeof(char));
	return strcpy(s, s_local);
}

/* 
 * Advances all of the obstacles in the screen. Recycles the ones that
 * have reached the end of the track.
 */
void advance_obstacles(
	struct WIN_pos_size win,
    struct pair_of_obstacles *pairs,
	unsigned int n_obstacles,
    struct obstacles_settings sett
) {
	unsigned int i;
	for (i=0; i<n_obstacles; i++) {
		if (pairs[i].x == 0) {
			recycle_obstacle(win, i, pairs, n_obstacles, sett);
		} else {
			pairs[i].x += -1;
        }
	}
}

/*
 * Prints a pair of obstacles.
 */
void print_obstacle(
	struct WIN_pos_size win,
    struct pair_of_obstacles *pairs,
	unsigned int i,
    struct obstacles_settings sett
) {
    if (
        pairs[i].x >= 0 &&
        pairs[i].x <= win.width - sett.width
    ) {
		char *block = get_obstacle_repr(sett.width);
		size_t j, k;
		for (j = 0; j <= pairs[i].top_obstacle.y; j++) {
			mvprintw(j, pairs[i].x, block);
        }
		for (k = pairs[i].bottom_obstacle.y; k < win.height; k++) {
			mvprintw(k, pairs[i].x, block);
        }
		free(block);
		refresh();
	}
}

/*
 * Prints all of the obstacles in the track.
 */
void print_obstacles(
    struct WIN_pos_size win,
    struct pair_of_obstacles *pairs,
    unsigned int n_obstacles,
    struct obstacles_settings sett
) {
    for (int i=0; i<n_obstacles; i++) {
        print_obstacle(win, pairs, i, sett);
    }
}

