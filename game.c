#include <pthread.h>
#include <curses.h>
#include <stdlib.h>

#define FPS 15 // frames per second

// using point as a simplified version of an obstacle.
struct point {
	unsigned char x;
	unsigned char y;
};

unsigned char alive = 1;
struct point po = {0, 0};

char * get_point_string(void) {
	char * s = "@";
	return s;
}

void advance_point(void) {
	po.x += 1;
}

void periodic_events(void) {
	advance_point();
}

void start_game(void) {
	while(alive) {
		clear();
		periodic_events();
		mvprintw(po.y, po.x, get_point_string());
		refresh();
		long loop_time = 1000000 / FPS; // 1000000 microsecs = 1s
		usleep(loop_time);
	}

	pthread_exit(NULL);
}
