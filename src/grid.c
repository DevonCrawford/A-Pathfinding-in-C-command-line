#include <stdio.h>
#include <stdlib.h>
#include "funcDefns.h"

// Calculates the size of the grid
GridSize getGridSize(FILE *file) {
	char c = '\0';
	char pc = '\0';
	int x, y = 0;
	GridSize size;

	if(file != NULL) {
		// Gets the length of the first line
		for(x = 0; (c = fgetc(file)) && (c != '\n' && !feof(file)); x++);

		// Resets file pointer to start of file
		rewind(file);

		// Gets the number of lines in the file
		while((c = fgetc(file)) && !feof(file)) {
			// Save previous character, check for blank newline bug
			pc = c;

			// If character is newline, add newline
			if(c == '\n') {
				y++;
			}
		}

		// Adds one to the line count
		if(x > 0 && pc != '\n') {
			y++;
		}
	}

	size.width = x;
	size.height = y;
	return size;
}

// Clears each character in grid to '0'
void clearGrid(char grid[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			grid[h][w] = '0';
		}
	}
}

// Prints grid characters as they appear
void printGrid(char grid[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			printf("%c", grid[h][w]);
		}
		printf("\n");
	}
}

// Copies one grid to another, used for solvedGrid
void copyGrid(char copy[][size.width], char grid[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			copy[h][w] = grid[h][w];
		}
	}
}

// Replaces path locations with spaces in solvedGrid
void insertPath(char grid[][size.width], Node path[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			if(path[h][w].exists == 1) {
				// Replaces a path location with a space
				grid[h][w] = ' ';
			}
		}
	}
}
