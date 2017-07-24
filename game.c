#include <pthread.h>
#include <curses.h>
#include <stdlib.h>

#include "obstacles.h"
#include "keys.h"

#define FPS 15 // frames per second
#define SECOND_IN_MICROSECONDS 1000000
#define SPEED SECOND_IN_MICROSECONDS / FPS


unsigned char alive = 1;

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
	free_obstacles();
}
