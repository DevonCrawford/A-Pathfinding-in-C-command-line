#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcDefns.h"

// Reads grid from file
// Returns 0 if correct and 1 if grid is incomplete
int readFile(FILE *file, char grid[][size.width]) {
	char c = '\0';
	int h, w;

	rewind(file);
	for(h = 0; h < size.height; h++) {
		for(w = 0; (c = fgetc(file)) && w < size.width && !feof(file); w++) {
			// Detects if the grid is incomplete, prints location and returns 1 (error)
			if(c == '\n' || (c != '#' && c != 'O' && c != 'S' && c != 'G')) {
				printf("Error: Grid data is incomplete at x: %d, y: %d\n", w + 1, h + 1);
				return 1;
			}
			// Saves file character to grid
			grid[h][w] = c;
		}
	}
	return 0;
}

// Writes the solution file
void writeFile(char grid[][size.width], char origName[]) {
	int h, w;
	FILE *file;

	// Declaring strings for new file name
	char insert[] = "_solution";
	char newName[strlen(origName) + strlen(insert)];

	// Adds solution onto origName, keeping extension intact
	appendName(origName, insert, newName);

	// Opens file with new name for writing
	file = fopen(newName, "w");

	// Writes each character of solved grid to new file
	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			fputc(grid[h][w], file);
		}
		fputc('\n', file);
	}
	// Close file
	fclose(file);
}

// Adds string to the end of origName, while keeping extension intact
void appendName(char origName[], char insert[], char newName[]) {
	// Declaring lengths of strings
	int nameLen = strlen(origName);
	int insertLen = strlen(insert);
	int newNameLen = nameLen + insertLen;

	// Declaring index counters
	int ri = -1;
	int ei = -1;
	int c;

	// Initializing every character in newName string to '\0'
	initString(newName);

	// Gets the location to replace (finds start of extension)
	for(ri = nameLen; ri >= 0 && origName[ri] != '.'; ri--);

	// If file does not have an extension
	if(ri == 0) {
		// Replace index is very end of string
		ri = nameLen - 1;
	}
	// extension index starts at replace index
	ei = ri;

	// Pieces newName together
	for(c = 0; c < newNameLen; c++) {
		// Saves newName as original from start up till replace index
		if(c < ri) {
			newName[c] = origName[c];
		}
		// Appends insert to newName
		else if(c < ri + insertLen) {
			newName[c] = insert[c - ri];
		}
		// Appends extension from origName to newName
		else if(c < newNameLen) {
			newName[c] = origName[ei];
			ei++;
		}
	}
	// Adds null terminator to end of string
	newName[c] = '\0';
}

// Initializes each character in string to null terminator
void initString(char string[]) {
	int w;
	int length = strlen(string);
	for(w = 0; w < length; w++) {
		string[w] = '\0';
	}
}
