#include <curses.h>
#include <stdlib.h>
#include <string.h>

// Function to calculate the maximum width and height of a multi-line string
void calculate_maxwidth_height(const char* string, int* maxwidth, int* height) {
    if (string == NULL) {
        *maxwidth = 0;
        *height = 0;
        return;
    }

    int i = 0, current_width = 0;
    *maxwidth = 0;
    *height = 1;

    while (string[i] != '\0') {
        if (string[i] == '\n') {
            (*height)++;
            *maxwidth = (*maxwidth > current_width) ? *maxwidth : current_width;
            current_width = 0;
        } else {
            current_width++;
        }
        i++;
    }
    *maxwidth = (*maxwidth > current_width) ? *maxwidth : current_width;
}

// Function to print a multi-line string in the middle of the screen
void print_string_middle_screen(char* string) {
    if (string != NULL) {
        int mxwidth, height;
        calculate_maxwidth_height(string, &mxwidth, &height);

        int y = (LINES / 2) - (height / 2);
        int x = (COLS / 2) - (mxwidth / 2);

        char* copy = strdup(string); // Make a writable copy
        char* token = strtok(copy, "\n");

        while (token != NULL) {
            mvprintw(y, x, "%s", token);
            y++; // Move to the next line
            token = strtok(NULL, "\n");
        }
    }
}

