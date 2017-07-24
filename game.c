#include <pthread.h>
#include <curses.h>
#include <stdlib.h>

#include "obstacles.h"
#include "keys.h"

#define FPS 15 // frames per second
#define SECOND_IN_MICROSECONDS 1000000
#define SPEED SECOND_IN_MICROSECONDS / FPS


struct bird {
	char x;
	char y;
};

pthread_mutex_t bird_y_mutex;

unsigned char alive = 1;

struct bird b = {10, 2};

char *get_bird_repr(void) {
	return ">>>";
}

void accelerate_bird(void) {
	pthread_mutex_lock(&bird_y_mutex);
	b.y += 1; 
	pthread_mutex_unlock(&bird_y_mutex);
}

void print_bird(void) {
	mvprintw(b.y, b.x, get_bird_repr());
	refresh();
}

void periodic_events(void) {
	accelerate_bird();
	advance_obstacles();
}

void *listen_controller(void * arg) {
	while(alive) {
		if (getch() == SPACEBAR) {
			pthread_mutex_lock(&bird_y_mutex);
			b.y -= 1; 
			pthread_mutex_unlock(&bird_y_mutex);
		}
	}

	pthread_exit(NULL);
}

void init_controller_listener(void) {
	int rc;
	pthread_t tid;
	pthread_mutex_init(&bird_y_mutex, NULL);
	rc = pthread_create(&tid, NULL, &listen_controller, NULL);
	if (rc) {
		fprintf(stderr, "cant create new thread! error code %d", rc);
		exit(1);
	}
}

void print_screen(void) {	
	clear();
	print_obstacles();
	print_bird();
}

void start_game(void) {
	init_obstacles();
	init_controller_listener();
	while(alive) {
		print_screen();
		periodic_events();
		usleep(SPEED);
	}
	
	pthread_mutex_destroy(&bird_y_mutex);
	free_obstacles();
}
