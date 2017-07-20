#include <curses.h>
#include <stdio.h>
#include <stdlib.h>


void print_file(char *filename, unsigned int x, unsigned int y) {
	FILE *fp;
	if ((fp = fopen(filename, "r")) != NULL) {
		char final_str[count_chars(fp)];
		size_t i = 0;
		int c;
		for (c = getc(fp); c != EOF; c = getc(fp))
			final_str[i++] = c;
		mvprintw(y, x, final_str);
		fclose(fp);
	} else {
		fprintf(stderr, "cant open file: %s", filename);
		exit(1);
	}
}

int count_chars(FILE *fp) {
	unsigned int chars = 0;
	int c;
	for (c = getc(fp); c != EOF; c = getc(fp))
		chars++;
	if (fseek(fp, 0L, SEEK_SET) != 0) {
		printf("Repositioning error.");
		exit(1);
	}
	return chars;
}

void print_title_screen() {
	#define FFLAPPY_SRC "res/title.txt"
	#define INST_SRC "res/instructions.txt"
	#define CREDITS_SRC "res/credits.txt"
	print_file(FFLAPPY_SRC, 0, 0);
	print_file(INST_SRC, 0, 20);
	print_file(CREDITS_SRC, 0, 30);
}


int main (void) {
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	
	print_title_screen();
	
	refresh();
	sleep(2);
	endwin();
}
