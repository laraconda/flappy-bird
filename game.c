#include <pthread.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "externs.h"
#include "obstacles.h"
#include "keys.h"

#define FPS 15 // frames per second
#define SECOND_IN_MICROSECONDS 1000000
#define SPEED SECOND_IN_MICROSECONDS / FPS

#define MAX_ACC 4.0


struct bird {
	int x;
	int y;
	float acc;
};

pthread_mutex_t bird_y_mutex;

unsigned char alive = 1;
unsigned long score = 0;

struct bird b = {10, 2, 1.0};

char *get_bird_repr(void) {
	return ">>>";
}

void accelerate_bird(void) {
	if (b.acc < MAX_ACC) {
		pthread_mutex_lock(&bird_y_mutex);
		b.acc += 0.6; 
		pthread_mutex_unlock(&bird_y_mutex);
	}
}

void move_bird(void) {
	b.y += (int)round(b.acc);
}

void print_bird(void) {
	mvprintw(b.y, b.x, get_bird_repr());
	refresh();
}

char* get_score_message(void) {
	char *score_s_template = "SCORE: %lu";
	int buff_size = snprintf(NULL, 0, score_s_template, score);
	char *score_s = malloc(buff_size + 1);
	snprintf(score_s, buff_size + 1, score_s_template, score);
	return score_s;
}

void update_score_window(void) {
	wclear(wscore);
	char *score_message = get_score_message();
	mvwprintw(wscore, 0, 0, score_message);
	wrefresh(wscore);
	free(score_message);
}

void refresh_score(void) {
	if (is_there_any_obstacle_at_x(b.x)) {
		score += 1;
		update_score_window();
	}
}

unsigned char is_bird_outside_bounds(void) {
	// Not checking x because x its fixed.
	return (b.y < 0 || b.y >= LINES);
}

void check_dead(void) {
	if (is_bird_outside_bounds())
		alive = 0;
}

void periodic_events(unsigned int i, unsigned int n_time_chunks) {
	accelerate_bird();
	advance_obstacles();
	move_bird();
	refresh_score();
	check_dead();
}

void *listen_controller(void * arg) {
	while(alive) {
		if (getch() == SPACEBAR) {
			pthread_mutex_lock(&bird_y_mutex);
			b.acc = -3.0; 
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


void print_score_message(void) {
	char *score_message = get_score_message();
	mvprintw((LINES/2) + 1, COLS/2, score_message);
	refresh();
	free(score_message);
}

void print_death_message(void) {
	clear();
	mvprintw(LINES/2, COLS/2, "YOU JUST DIED BOI");
	print_score_message();	
	refresh();
	sleep(2);
}

void set_up_windows(void) {
	wresize(stdscr, LINES - 1, COLS);
   // WINDOW *wscore = NULL;
	wscore = newwin(1, COLS, LINES - 1, 0);
}

void start_game(void) {
	set_up_windows();
	update_score_window();

	init_obstacles();
	init_controller_listener();
	while(alive) {
		unsigned int n_time_chunks = 10;
		static unsigned int i;
		if (i++ == n_time_chunks)  // dividing time in n_time_chunks
			i = 0;
		
		print_screen();
		periodic_events(i, n_time_chunks);
		
		usleep(SPEED);
	}
	
	pthread_mutex_destroy(&bird_y_mutex);
	free_obstacles();
	print_death_message();
}
