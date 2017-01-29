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
void writeFile(char grid[][size.width], char origName[]);

void addListNode(Node list[][size.width], int* length, Node node);
void removeListNode(Node list[][size.width], int *length, Node node);
void clearGrid(char grid[][size.width]);
void printGrid(char grid[][size.width]);
void copyGrid(char copy[][size.width], char grid[][size.width]);
void initNodes(Node node[][size.width]);
void clearNode(Node *node);
void nodesExistence(Node node[][size.width]);
Node getNodeChar(char grid[][size.width], char toFind);
Node getNode(Node[][size.width], int openLength);
void arrangeID(Node arrange[], Node original[][size.width], int length);
void insertPath(char grid[][size.width], Node path[][size.width]);

void info(Node node);
void info1D(Node nodes[], int length);
void info2D(Node nodes[][size.width], int length);

Node lowestF(Node open[][size.width], int length);
void findPath(char grid[][size.width], Node open[][size.width], Node closed[][size.width], Node path[][size.width], Node start, Node goal, Node parent, int* openLength, int* closedLength, int* pathLength);
void calculateNodeValues(Node parent, Node goal, int possibleX, int possibleY, char grid[][size.width], Node open[][size.width], int* openLength);
void connectPath(Node path[][size.width], int *pathLength, Node parent);
void reverseID(Node path[][size.width], int pathLength);

int main(int argc, char* argv[]) {

	FILE *file = fopen(argv[1], "r");
	int gridError = 0;

	if(file != NULL) {
		size = getGridSize(file);

		char grid[size.height][size.width];
		char solvedGrid[size.height][size.width];

		clearGrid(grid);

		gridError = readFile(file, grid);

		// If there is no error reading grid from file
		if(gridError == 0) {
			// Makes a copy of the grid for the solution
			copyGrid(solvedGrid, grid);

			// Prints the original map
			printGrid(grid);

			// Declaring Node arrays
			Node open[size.height][size.width];
			Node closed[size.height][size.width];
			Node path[size.height][size.width];

			// Initializing Node arrays
			initNodes(open);
			initNodes(closed);
			initNodes(path);

			int openLength = 0;
			int closedLength = 0;
			int pathLength = 0;

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

				nodesExistence(path);
				info2D(path, pathLength);
				printf("SOLVED:..\n");
				printGrid(solvedGrid);

				writeFile(solvedGrid, argv[1]);
			}
		}
	}
	else {
		printf("Error: \"%s\" does not exist.\n", argv[1]);
	}

	return 0;
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

// Clears the node to default values
void clearNode(Node *node) {
	node->parent = NULL;
	node->exists = 0;
	node->id = -1;
	node->f = -1;
	node->g = -1;
	node->h = -1;
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

Node lowestF(Node open[][size.width], int length) {
	// Copies open nodes to new array for sorting
	Node sort[length];
	arrangeID(sort, open, length);

	int Switch = -1;
	Node curr, next;
	int i;

	while(Switch != 0) {
		Switch = 0;

		for(i = 0; i < length - 1; i++) {
			curr = sort[i];
			next = sort[i + 1];

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
	parent = lowestF(open, *openLength);

	// If chosen parent does not exist..
	// Must be no path, exhausted entire map
	if(parent.exists != 1) {
		printf("NO PATH U BETCH\n");
		return;
	}
	// If parent node is the goal!
	else if(grid[parent.y][parent.x] == 'G') {
		printf("\nfound path!!\n");

		Node lastClosed = getNode(closed, (*closedLength)-1);
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

		// Parent of parent is saved
		parent = *(parent.parent);
	}
	// Flips id's in path because they were recorded in reverse
	reverseID(path, *pathLength);
}

void reverseID(Node path[][size.width], int pathLength) {
	int s, e = pathLength - 1;
	Node start, end;

	for(s = 0; s < e; s++) {
		start = getNode(path, s);
		end = getNode(path, e);

		path[start.y][start.x].id = e;
		path[end.y][end.x].id = s;
		e--;
	}
}

// Reads grid from file
// Returns 0 if correct and 1 if grid is incomplete
int readFile(FILE *file, char grid[][size.width]) {
	char c = '\0';
	int h, w;

	rewind(file);
	for(h = 0; h < size.height; h++) {
		for(w = 0; (c = fgetc(file)) && w < size.width && !feof(file); w++) {
			if(c == '\n') {
				printf("Error: Grid data is incomplete at x: %d, y: %d\n", w + 1, h + 1);
				return 1;
			}
			grid[h][w] = c;
		}
	}
	return 0;
}

void initString(char string[]) {
	int length = strlen(string);
	int w;
	for(w = 0; w < length; w++) {
		string[w] = '\0';
	}
}

void writeFile(char grid[][size.width], char origName[]) {
	int nameLen = strlen(origName);
	int newNameLen = nameLen + 8;

	char name[newNameLen];
	initString(name);
	char addon[] = "solution";
	int el = -1;
	int a = -1;
	int ri = -1;
	int ari = -1;
	FILE *file;

	// Gets the location to replace
	for(ri = nameLen; origName[ri] != '.'; ri--);

	ari = ri + 8;
	el = nameLen - ri + 1;
	char extension[el];
	initString(extension);

	for(a = ri; a < nameLen; a++) {
		extension[a - ri] = origName[a];
	}
	extension[a - ri] = '\0';

	int w;
	for(w = 0; w < newNameLen; w++) {
		if(w < ri) {
			name[w] = origName[w];
		}
		else if(w < ari){
			name[w] = addon[w - nameLen + el - 1];
		}
		else if (w < ari + el){
			name[w] = extension[w - ari];
		}
	}
	name[w] = '\0';

	printf("Filename: %s\n", name);
	file = fopen(name, "w");

	int h;
	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			fputc(grid[h][w], file);
		}
		fputc('\n', file);
	}
	fclose(file);
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
