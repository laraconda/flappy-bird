#include <pthread.h>
#include <curses.h>
#include <stdlib.h>

#include "keys.h"

#define FPS 15 // frames per second
#define SECOND_IN_MICROSECONDS 1000000
#define SPEED SECOND_IN_MICROSECONDS / FPS

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

void *listen_controller(void * arg) {
	while(alive) {
		if (getch() == SPACEBAR) {
			po.y += 1;
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

	mvprintw(po.y, po.x, get_point_string());
	
	refresh();
}

void start_game(void) {
	init_controller_listener();
	while(alive) {
		print_screen();
		periodic_events();
		usleep(SPEED);
	}

	pthread_exit(NULL);
}
