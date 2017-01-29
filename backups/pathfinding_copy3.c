#include<stdio.h>
#include<stdlib.h>
#include<math.h>
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
void clearGrid(char grid[][size.width]);
void printGrid(char grid[][size.width]);
void clearNode(Node *node);
void initNodes(Node node[][size.width]);
void printNodes(Node node[][size.width]);
void findNodes(char grid[][size.width], Node node[][size.width], char toFind);

void printSort(Node sort[], int length);
void printNodeInfo(Node nodes[][size.width]);

Node getNodeChar(char grid[][size.width], char toFind);
Node getNode(Node[][size.width], int openLength);
void addListNode(Node list[][size.width], int* length, Node node, char debug[]);
void findPath(char grid[][size.width], Node open[][size.width], Node closed[][size.width], Node path[][size.width], Node start, Node goal, Node parent, int* openLength, int* closedLength, int* pathLength);
void calculateNodeValues(Node parent, Node goal, int possibleX, int possibleY, char grid[][size.width], Node open[][size.width], int* openLength);
void connectPath(char grid[][size.width], Node path[][size.width], Node parent, int *pathLength);

int main(int argc, char* argv[]) {

	FILE *file = fopen(argv[1], "r");
	int gridError = 0;

	if(file != NULL) {
		size = getGridSize(file);

		char grid[size.height][size.width];

		clearGrid(grid);

		gridError = readFile(file, grid);

		// If there is no error reading grid from file
		if(gridError == 0) {
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

			/* Parents */
			// Node parent, parentParent;
			// Node test;
			// parentParent.g = 20;
			// parent.g = 10;
			//
			// open[0][0].parent = &parent;
			// open[0][0].parent->parent = &parentParent;
			// // PROBLEM:: HERE IT WORKS..
			// // In printParents it doesnt. setting parent
			// // Nodes must be off in open Node creation
			// // Or while switching to closed Nodes
			// test = *(open[0][0].parent);
			//
			// printf("Parent: %d\n", open[0][0].parent->g);
			// printf("Parent(x2): %d\n", open[0][0].parent->parent->g);
			// printf("ParentTONode: %d\n", test.g);

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
				// Adds the start node to the closed list
				addListNode(closed, &closedLength, start, "START");

				findPath(grid, open, closed, path, start, goal, start, &openLength, &closedLength, &pathLength);
				printNodes(closed);
			}
		}
	}
	else {
		printf("Error: \"%s\" does not exist.\n", argv[1]);
	}

	return 0;
}

void addListNode(Node list[][size.width], int* length, Node node, char debug[]) {
	// Crashes program
	// Node parent = *(node.parent);

	if(node.parent != NULL) {
		printf("ALN %s\n%d %d\n", debug, node.parent->x, node.parent->y);
	}

	list[node.y][node.x] = node;
	list[node.y][node.x].exists = 1;
	list[node.y][node.x].id = (*length);
	(*length)++;

	//printf("alN x: %d, y: %d, E: %d. Px: %d, Py: %d, E: %d\n", list[node]);
	// printNodeInfo(list);
	// printf("\n");
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
	node->exists = 0;
	node->id = -1;
	node->f = -1;
	node->g = -1;
	node->h = -1;
}

void findNodes(char grid[][size.width], Node node[][size.width], char toFind) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			if(grid[h][w] == toFind) {
				node[h][w].exists = 1;
			}
		}
	}
}

// Prints 1 if Node at that position exists
// Prints 0 if Node at that position does not exist
void printNodes(Node node[][size.width]) {
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
	Node start;
	clearNode(&start);

	for(h = 0; h < size.width; h++) {
		for(w = 0; w < size.width; w++) {
			// If start is found, save the position and return Node start
			if(grid[h][w] == toFind) {
				start.x = w;
				start.y = h;
				start.exists = 1;
				return start;
			}
		}
	}
	return start;
}

// Finds node with specific id number index and returns it
Node getNode(Node list[][size.width], int findIndex) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			if(list[h][w].id == findIndex) {
				//printf("n.. %d, %d, E: %d //// nParent: %d, %d, E: %d\n" ,list[h][w].x, list[h][w].y, list[h][w].exists, list[h][w].parent->x, list[h][w].parent->y, list[h][w].parent->exists);
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
					//printf("a.. %d, %d, E: %d //// aParent: %d, %d, E: %d\n" ,original[h][w].x, original[h][w].y, original[h][w].exists, original[h][w].parent->x, original[h][w].parent->y, original[h][w].parent->exists);
					//arrange[a].parent = original[h][w].parent;
				}
			}
		}
	}
}

void printNodeInfo(Node nodes[][size.width]) {
	int h, w;

	for(h = 0; h < size.height; h++) {
		for(w = 0; w < size.width; w++) {
			if(nodes[h][w].exists == 1) {
				printf("info.. %d, %d, E: %d //// Parent: %d, %d, E: %d\n" , nodes[h][w].x, nodes[h][w].y, nodes[h][w].exists, nodes[h][w].parent->x, nodes[h][w].parent->y, nodes[h][w].parent->exists);
			}
		}
	}
}

void printSort(Node sort[], int length) {
	int i;

	for(i = 0; i < length; i++) {
		printf("s.. %d, %d, E: %d //// sParent: %d, %d, E: %d\n", sort[i].x, sort[i].y, sort[i].exists, sort[i].parent->x, sort[i].parent->y, sort[i].parent->exists);
	}
	printf("\n");
}

Node lowestFCost(Node open[][size.width], int length) {
	// If there is only one open Node, return it
	if(length == 1) {
		return getNode(open, 0);
	}
	// Copies open nodes to new array for sorting
	Node sort[length];
	arrangeID(sort, open, length);

	// int w;
	// for(w = 0; w < length; w++) {
	// 	Node p = getNode(open, w);
	// 	printf("o.. %d, %d, E: %d //// oParent: %d, %d, E: %d\n", p.x, p.y, p.exists, p.parent->x, p.parent->y, p.parent->exists);
	// }

	int Switch = -1;
	Node curr, next;
	int i;

	while(Switch != 0) {
		Switch = 0;

		for(i = 0; i < length - 1; i++) {
			curr = sort[i];
			next = sort[i + 1];

			if(curr.f > next.f) {
				sort[curr.id] = next;
				sort[next.id] = curr;
				Switch = 1;
			}
		}
		/*int w = 0;
		for(w = 0; w < length; w++) {
			Node p = getNode(open, w);
			printf("o.. %d, %d, E: %d //// oParent: %d, %d, E: %d\n", p.x, p.y, p.exists, p.parent->x, p.parent->y, p.parent->exists);
		}
		//printSort(sort, length); */
	}
	//printf("PS: %d %d\n", sort[0].parent->x, sort[0].parent->y);
	// Returns lowest F cost node
	//printf("s.. %d, %d, E: %d //// sParent: %d, %d, E: %d\n", sort[0].x, sort[0].y, sort[0].exists, sort[0].parent->x, sort[0].parent->y, sort[0].parent->exists);
	return sort[0];
}


void printParents(Node nodes[][size.width], int length) {
	//int a;
	Node parent = getNode(nodes, length-1);

	//printf("P: %d %d\n", parent.x, parent.y);
	printf("LEN: %d\n", length);

	//for(a = 0; a < length-1; a++) {
		printf("Px: %d, Py: %d.Pe: %d\n", parent.x, parent.y, parent.exists);
		parent = *(parent.parent);
		printf("PPx: %d, PPy: %d.. PPe: %d\n", parent.x, parent.y, parent.exists);
		//*(struct_inst->pointer) = var;
	//}
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
	// printNodeInfo(open);
	// printf("\n");
	// int w;
	// for(w = 0; w < *openLength; w++) {
	// 	Node p = getNode(open, w);
	// 	printf("o.. %d, %d, E: %d //// oParent: %d, %d, E: %d\n", p.x, p.y, p.exists, p.parent->x, p.parent->y, p.parent->exists);
	// }
	// Chooses open node with lowest F cost as new parent node

	Node copy[*openLength];
	arrangeID(copy, open, *openLength);

	parent = lowestFCost(open, *openLength);
	printf("\nSORT\n%d %d\n\n", parent.x, parent.y);

	// Remove parent from open list
	removeListNode(open, openLength, parent);

	// Adds parent to closed list
	addListNode(closed, closedLength, parent, "CLOSED");

	// If parent node is the goal!
	if(grid[parent.y][parent.x] == 'G') {
		//goal.parent = parent.parent;
		//printParents(open, *openLength);
		//Node p = *(goal.parent);
		printf("\nfound path: %d %d\n", parent.parent->x, parent.parent->y);
		//connectPath(grid, path, goal.parent, pathLength);
		return;
	}

	// Recursive call replacing original parent with new parent
	// if(*openLength + *closedLength < 10) {
		findPath(grid, open, closed, path, start, goal, parent, openLength, closedLength, pathLength);
	// }
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
	openNode.parent = &parent;

	// Node p = *(openNode.parent);
	// printf("PoX: %d, PoY: %d, E: %d\n", p.x, p.y, p.exists);

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

	// Prints information about the openNode
	//printf("OPENINFO.. x: %d, y: %d. Px: %d, Py: %d...g: %d, h: %d, f: %d\n", openNode.x, openNode.y, openNode.parent->x, openNode.parent->y, openNode.g, openNode.h, openNode.f);

	printf("\nOPEN\n%d %d\n\n", openNode.parent->x, openNode.parent->y);

	// Add the open node to the open list
	addListNode(open, openLength, openNode, "OPEN");

	//printNodeInfo(open);
	//printf("\n");
}


/*
void connectPath(char grid[][size.width], Node path[][size.width], Node parent, int *pathLength) {
	//while(grid[parent.y][parent.x] != 'S') {
		addListNode(path, pathLength, parent);
		//parent = *(parent.parent);
	//}
	printf("PATH\n");
	printNodes(path);
} */

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
