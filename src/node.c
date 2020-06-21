#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "funcDefns.h"

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

// Adds a node to a list by saving it's id, index in the list
void addListNode(Node list[][size.width], int* length, Node node) {
	list[node.y][node.x] = node;
	list[node.y][node.x].exists = 1;

	// Saves id as a representative of index in list
	list[node.y][node.x].id = (*length);
	(*length)++;
}

// Removes a node from a list
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
				// If node id matches the search criteria
				if(original[h][w].id == a) {
					// Saves node in arrange at index of node id
					arrange[a] = original[h][w];
					break;
				}
			}
		}
	}
}

// Reverses Node id's in a grid
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

// Calculates mathematical values on a single open node
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
