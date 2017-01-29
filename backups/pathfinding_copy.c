#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#define PI_2 3.14159/2

// Creating GridSize struct
typedef struct GridSize {
	int width;
	int height;
} GridSize;

// Creating Node struct
typedef struct Node {
	struct Node* parent;
	int exists;
	int x;
	int y;
	int g;
	int h;
	int f;
	int id;
} Node;

GridSize size;
GridSize getGridSize(FILE *file);
int readFile(FILE *file, char grid[][size.width]);
void initString(char string[]);
void appendName(char origName[], char insert[], char newName[]);
void writeFile(char grid[][size.width], char origName[]);
void clearGrid(char grid[][size.width]);
void printGrid(char grid[][size.width]);
void copyGrid(char copy[][size.width], char grid[][size.width]);

void clearNode(Node *node);
void initNodes(Node node[][size.width]);
void nodesExistence(Node node[][size.width]);
Node getNodeChar(char grid[][size.width], char toFind);
Node getNode(Node[][size.width], int openLength);
void addListNode(Node list[][size.width], int* length, Node node);
void removeListNode(Node list[][size.width], int *length, Node node);
void arrangeID(Node arrange[], Node original[][size.width], int length);
Node lowestF(Node open[][size.width], int length);
void findPath(char grid[][size.width], Node open[][size.width], Node closed[][size.width], Node path[][size.width], Node start, Node goal, Node parent, int* openLength, int* closedLength, int* pathLength);
void calculateNodeValues(Node parent, Node goal, int possibleX, int possibleY, char grid[][size.width], Node open[][size.width], int* openLength);
void connectPath(Node path[][size.width], int *pathLength, Node parent);
void reverseID(Node path[][size.width], int pathLength);
void insertPath(char grid[][size.width], Node path[][size.width]);

void info(Node node);
void info1D(Node nodes[], int length);
void info2D(Node nodes[][size.width], int length);

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
				insertPath(solvedGrid, path);

				printf("\nSOLVED:..\n");
				printGrid(solvedGrid);

				writeFile(solvedGrid, argv[1]);
			}
		}
	}
	// If file has any errors
	else {
		if(argc == 1) {
			printf("Error: No file was entered in command line\n");
		}
		else {
			printf("Error: \"%s\" does not exist.\n", argv[1]);
		}
	}
	return 0;
}

GridSize getGridSize(FILE *file) {
	char c = '\0';
	int x, y = 0;
	GridSize size;

	if(file != NULL) {
		// Gets the length of the first line
		for(x = 0; (c = fgetc(file)) && (c != '\n' && !feof(file)); x++);

		// Resets file pointer to start of file
		rewind(file);

		// Gets the number of lines in the file
		while((c = fgetc(file)) && !feof(file)) {
			if(c == '\n') {
				y++;
			}
		}

		// Adds one to the line count
		if(x > 0) {
			y++;
		}
	}

	size.width = x;
	size.height = y;
	return size;
}

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

// Initializes each character in string to null terminator
void initString(char string[]) {
	int w;
	int length = strlen(string);
	for(w = 0; w < length; w++) {
		string[w] = '\0';
	}
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

void writeFile(char grid[][size.width], char origName[]) {
	int h, w;
	FILE *file;

	// Declaring strings for new file name
	char insert[] = "solution";
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

void clearGrid(char grid[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			grid[h][w] = '0';
		}
	}
}

void printGrid(char grid[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			printf("%c", grid[h][w]);
		}
		printf("\n");
	}
}

void copyGrid(char copy[][size.width], char grid[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			copy[h][w] = grid[h][w];
		}
	}
}

// Clears the node to default values
void clearNode(Node *node) {
	node->parent = NULL;
	node->exists = 0;
	node->id = -1;
	node->f = -1;
	node->g = -1;
	node->h = -1;
}

// Initializes all nodes to default values
void initNodes(Node node[][size.width]) {
	int h, w;
	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			clearNode(&node[h][w]);
			node[h][w].x = w;
			node[h][w].y = h;
		}
	}
}

// Prints 1 if Node at that position exists
// Prints 0 if Node at that position does not exist
void nodesExistence(Node node[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			printf("%d", node[h][w].exists);
		}
		printf("\n");
	}
}

// Gets a node (with position) based upon the character on the grid
// Used for finding the start Node and goal Node
Node getNodeChar(char grid[][size.width], char toFind) {
	int h, w;
	Node node;
	clearNode(&node);

	for(h = 0; h < size.width; h++) {
		for(w = 0; w < size.width; w++) {
			// If char 'toFind' is found, save the position and return node
			if(grid[h][w] == toFind) {
				node.x = w;
				node.y = h;
				node.exists = 1;
				return node;
			}
		}
	}
	return node;
}

// Finds node with specific id number index and returns it
Node getNode(Node list[][size.width], int findIndex) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			if(list[h][w].id == findIndex) {
				return list[h][w];
			}
		}
	}
	Node fail;
	fail.exists = 0;
	fail.id = -1;
	return fail;
}

// Copies data from Node orig into parent of Node copy
void setParent(Node *copy, Node orig) {
    if(copy->parent == NULL) {
        copy->parent = (Node*) malloc(sizeof(Node));
    }
	copy->parent->parent = orig.parent;
    copy->parent->exists = orig.exists;
    copy->parent->x = orig.x;
    copy->parent->y = orig.y;
    copy->parent->g = orig.g;
    copy->parent->h = orig.h;
    copy->parent->f = orig.f;
    copy->parent->id = orig.id;
}

void addListNode(Node list[][size.width], int* length, Node node) {
	list[node.y][node.x] = node;
	list[node.y][node.x].exists = 1;
	list[node.y][node.x].id = (*length);
	(*length)++;
}

void removeListNode(Node list[][size.width], int *length, Node node) {
	int h, w;

	// If node exists
	if(node.exists == 1) {
		// If node is not the last in list
		if(node.id != (*length)-1) {
			// Iterates through every node in list
			for(h = 0; h < size.height; h++) {
				for(w = 0; w < size.width; w++) {
					// If node has id greater than one being removed
					if(list[h][w].id > node.id) {
						// Decrement id
						list[h][w].id--;
					}
				}
			}
		}
		// Clear values on the node
		clearNode(&list[node.y][node.x]);

		// Free parent from memory
        free(list[node.y][node.x].parent);

		// Decrement length
		(*length)--;
	}
}

// Arranges original nodes with a valid id number
// from low to high, into 1D array to be sorted
void arrangeID(Node arrange[], Node original[][size.width], int length) {
	int a, h, w;

	for(a = 0; a < length; a++) {
		for(h = 0; h < size.height; h++) {
			for(w = 0; w < size.width; w++) {
				if(original[h][w].id == a) {
					arrange[a] = original[h][w];
					break;
				}
			}
		}
	}
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
	return sort[0];
}

void findPath(char grid[][size.width], Node open[][size.width], Node closed[][size.width], Node path[][size.width], Node start, Node goal, Node parent, int* openLength, int* closedLength, int* pathLength) {
	int i;
	// Checks all directions (90 degrees)
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
		printf("No path found..\n");

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

void calculateNodeValues(Node parent, Node goal, int possibleX, int possibleY, char grid[][size.width], Node open[][size.width], int* openLength) {
	// Creates a new open node (to be looked at)
	Node openNode;
	clearNode(&openNode);

	// Declaring and initalizing variables used for calculations
	int gCost = 0;
	int hxDiff = 0;
	int hyDiff = 0;
	int hCost = 0;
	int fCost = 0;

	// Sets coordinates of the open node
	openNode.x = possibleX;
	openNode.y = possibleY;

	// Sets the parent of the open node
	setParent(&openNode, parent);

	// Calculating G cost
	// Cost to move from parent node to one open node
	gCost = parent.g + 1;
	openNode.g = gCost;

	// Calculating H cost
	// Estimated cost to move from the open node to the goal node
	hxDiff = fabs(goal.x - openNode.x);
	hyDiff = fabs(goal.y - openNode.y);
	hCost = hxDiff + hyDiff;
	openNode.h = hCost;

	// Calculating F cost (combined G and H)
	fCost = gCost + hCost;
	openNode.f = fCost;

	// Add the open node to the open list
	addListNode(open, openLength, openNode);
}

void connectPath(Node path[][size.width], int *pathLength, Node parent) {
	// Parent pointers are followed from goal until it reaches start
	while(parent.parent != NULL) {
		// Adds parent to the path list
		addListNode(path, pathLength, parent);

		// Parent Node is overwritten by parent Node of itself
		parent = *(parent.parent);
	}
	// Flips id's in path because they were recorded in reverse
	reverseID(path, *pathLength);
}

void reverseID(Node path[][size.width], int pathLength) {
	// s starts at 0, e starts at last index in path
	int s, e = pathLength - 1;
	Node start, end;

	// s increments while e decrements
	for(s = 0; s < e; s++) {
		// Gets node at id s and e
		start = getNode(path, s);
		end = getNode(path, e);

		// Swaps id's of nodes start and end
		path[start.y][start.x].id = e;
		path[end.y][end.x].id = s;
		e--;
	}
}

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

// Prints information about one node
void info(Node node) {
    if(node.parent != NULL) {
        printf("info.. %d, %d. f: %d. id: %d. E: %d ", node.x, node.y, node.f, node.id, node.exists);
        printf("//// Parent: %d, %d. f: %d. id: %d. E: %d\n", node.parent->x, node.parent->y, node.parent->f, node.parent->id, node.parent->exists);
    }
    else {
        printf("info.. %d, %d. f: %d. id: %d. E: %d //// Parent = NULL\n", node.x, node.y, node.f, node.id, node.exists);
    }
}

// Prints information of a one-dimensional array of nodes
void info1D(Node nodes[], int length) {
	int h;
	for(h = 0; h < length; h++) {
		if(nodes[h].exists == 1) {
            info(nodes[h]);
		}
	}
}

// Prints information of a two-dimensional (grid) array of nodes
void info2D(Node nodes[][size.width], int length) {
	int a, h, w;

	for(a = 0; a < length; a++) {
		for(h = 0; h < size.height; h++) {
			for(w = 0; w < size.width; w++) {
				if(nodes[h][w].id == a) {
		            info(nodes[h][w]);
					break;
				}
			}
		}
	}
}
