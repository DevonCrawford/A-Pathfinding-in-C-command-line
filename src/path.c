#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include <sys/timeb.h>
#include"funcDefns.h"
#define PI_2 3.14159/2

/* path.c
   This program reads maze data from a file, solves
   the shortest path, and outputs the solution to a new file.
   Currently the algorithm uses four directions (90 degrees)
   but can be easily modified to support any angles since
   the node generation is based upon sin and cos functions
   by Devon Crawford (dcrawf04@mail.uoguelph.ca)
   0973673
   Dec 2. 2016
*/

int main(int argc, char* argv[]) {
	// Attempts to open file
	FILE *file = fopen(argv[1], "r");
	int gridError = 0;

	// If file exists
	if(file != NULL) {
		// Getting the size of the grid from file
		size = getGridSize(file);

		// Declaring grid arrays based on pre calculated size
		char grid[size.height][size.width];
		char solvedGrid[size.height][size.width];

		// Initializes grid
		clearGrid(grid);

		// Reads file into grid and detects any errors
		gridError = readFile(file, grid);

		// If there is no error reading grid from file
		if(gridError == 0) {
			struct timeb startTime, endTime;
    		int diff;
			ftime(&startTime);

			// Makes a copy of the grid for the solution
			copyGrid(solvedGrid, grid);

			// Prints the original map
			printf("=== Original Grid ===\n");
			printGrid(grid);

			// Declaring Node arrays
			Node open[size.height][size.width];
			Node closed[size.height][size.width];
			Node path[size.height][size.width];

			// Initializing Node arrays
			initNodes(open);
			initNodes(closed);
			initNodes(path);

			// Declaring and initializing lengths for Node arrays
			int openLength = 0;
			int closedLength = 0;
			int pathLength = 0;

			// Gets the node of start and goal
			Node start = getNodeChar(grid, 'S');
			Node goal = getNodeChar(grid, 'G');

			// If start node was not found
			if(start.exists == 0) {
				printf("Error: Start node was not found.\n");
			}
			// If goal node was not found
			if(goal.exists == 0) {
				printf("Error: Goal node was not found.\n");
			}

			// If start and goal nodes both exist
			if(start.exists == 1 && goal.exists == 1) {
				// Add start node to the closed list
				addListNode(closed, &closedLength, start);

				findPath(grid, open, closed, path, start, goal, start, &openLength, &closedLength, &pathLength);

				// If path was found..
				if(pathLength > 0) {
					// Inserts spaces into solvedGrid
					insertPath(solvedGrid, path);

					// Recording endtime, Calculating runtime (diff)
					ftime(&endTime);
		    		diff = (int) (1000.0 * (endTime.time - startTime.time) +
					(endTime.millitm - startTime.millitm));

					// Prints completion time
		    		printf("\nSOLVED.. in %u milliseconds\n", diff);

					// Prints solved grid
					printGrid(solvedGrid);

					// Writes solution file
					writeFile(solvedGrid, argv[1]);
				}
			}
		}
	}
	// If file has any errors
	else {
		if(argc == 1) {
			printf("Error: No maze file was entered in command line. Correct input: \"programName mazeName.txt\"\n");
		}
		else {
			printf("Error: \"%s\" does not exist.\n", argv[1]);
		}
	}
	return 0;
}

// Returns node with lowest F value
Node lowestF(Node open[][size.width], int length) {
	// Copies open nodes to new 1D array for sorting
	Node sort[length];
	arrangeID(sort, open, length);

	int Switch = -1;
	Node curr, next;
	int i;

	// Sorts open array from low to high
	while(Switch != 0) {
		Switch = 0;

		// Iterates each existing open node (length)
		for(i = 0; i < length - 1; i++) {
			curr = sort[i];
			next = sort[i + 1];

			// SWAP: if curr Node has greater f than next Node
			if(curr.f > next.f) {
				sort[i] = next;
				sort[i + 1] = curr;
				Switch = 1;
			}
		}
	}
	// Returns node with lowest f
	return sort[0];
}

// Main recursive function to find open nodes, new parent, and repeat
void findPath(char grid[][size.width], Node open[][size.width], Node closed[][size.width], Node path[][size.width], Node start, Node goal, Node parent, int* openLength, int* closedLength, int* pathLength) {
	int i;
	// Checks all 4 directions
	for(i = 0; i < 4; i++) {
		// Uses cosine and sine functions to get circle of points (at 90 degree intervals) around parent
		int possibleX = parent.x + round(-1 * cos(PI_2 * i));
		int possibleY = parent.y + round(-1 * sin(PI_2 * i));

		// If coordinates are outside of boundaries
		if(possibleX < 0 || possibleY < 0 || possibleX > (size.width - 1) || possibleY > (size.height - 1)) {
			continue;
		}

		// Does not make a new open node IF
		// Node is a border, node is already in the open list
		// Or the node is already in the closed list
		if(grid[possibleY][possibleX] == '#' || closed[possibleY][possibleX].exists == 1 || open[possibleY][possibleX].exists == 1) {
			continue;
		}
		// Create an open node, calculate the values and add it to the open list
		calculateNodeValues(parent, goal, possibleX, possibleY, grid, open, openLength);
	}
	// Sets new parent as openNode with the lowest F cost
	parent = lowestF(open, *openLength);

	// If chosen parent does not exist..
	// Must be no path, exhausted entire map
	if(parent.exists != 1) {
		// Inform user that no path was found
		printf("\n!! No path exists !!\n");

		// Breaks out of entire pathfinding functions
		return;
	}
	// If parent node is the goal!
	else if(grid[parent.y][parent.x] == 'G') {
		// Get the last node in the closed list
		Node lastClosed = getNode(closed, (*closedLength)-1);

		// Connect path by tracing back parents, starting at lastClosed Node
		connectPath(path, pathLength, lastClosed);
		return;
	}

	// Remove parent from open list
	removeListNode(open, openLength, parent);

	// Adds parent to closed list
	addListNode(closed, closedLength, parent);

	// Recursive call to function, replacing parent as the open node with lowest fCost
	findPath(grid, open, closed, path, start, goal, parent, openLength, closedLength, pathLength);
}

// Parent pointers are followed from goal until it reaches start
void connectPath(Node path[][size.width], int *pathLength, Node parent) {
	while(parent.parent != NULL) {
		// Adds parent to the path list
		addListNode(path, pathLength, parent);

		// Parent Node is overwritten by parent Node of itself
		parent = *(parent.parent);
	}
	// Flips id's in path because they were recorded in reverse
	reverseID(path, *pathLength);
}
