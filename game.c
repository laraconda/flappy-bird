#include <pthread.h>
#include <curses.h>
#include <stdlib.h>

#define SPEED 5
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
	po.x += SPEED * 1;
}

void periodic_events(void) {
	advance_point();
}

void *game_clock(void * arg) {
	while(alive) {
		sleep(1);
		periodic_events();
	}

	pthread_exit(NULL);
}

void init_game_clock(void) {
	int rc;
	pthread_t tid;
	rc = pthread_create(&tid, NULL, &game_clock, NULL);
	if (rc) {
		fprintf(stderr, "cant create new thread! error code %d", rc);
		exit(1);
	}
}

void start_game(void) {
	init_game_clock();
	while(alive) {
		clear();
		mvprintw(po.y, po.x, get_point_string());
		refresh();
		long loop_time = 1000000 / FPS; // 1000000 microsecs = 1s
		usleep(loop_time);
	}

	pthread_exit(NULL);
}
