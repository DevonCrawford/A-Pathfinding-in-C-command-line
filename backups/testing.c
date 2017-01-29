#include<stdio.h>
#include<stdlib.h>
#define size 6

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

void initNodes(Node node[]);
void clearNode(Node *node);
void info(Node node);
void nodesInfo(Node nodes[]);
Node getNode(Node list[], int findIndex);
Node lowestF(Node open[], int length);
void arrangeID(Node arrange[], Node original[], int length);
void addListNode(Node list[], int* length, Node node);
void removeListNode(Node list[], int *length, Node node);
void setParent(Node *parent, Node orig);

int main(void) {
    Node open[size];
    int openLength = 0;
    //int closedLength = 0;

    initNodes(open);

    open[0].f = 31;
    open[1].f = 8;
    open[2].f = 1;
    open[3].f = 32;
    open[4].f = 16;
    open[5].f = 2;

    int i;
    // Adds all nodes to open list
    for(i = 0; i < size; i++) {
        addListNode(open, &openLength, open[i]);
    }
    setParent(&open[1], open[0]);
    setParent(&open[2], open[1]);
    setParent(&open[3], open[2]);
    setParent(&open[4], open[3]);
    setParent(&open[5], open[4]);

    nodesInfo(open);

    Node find = getNode(open, 2);
    printf("\nFind.. id 2\n");
    info(find);
    printf("\n");

    Node lowF = lowestF(open, size);
    printf("\nLOW\n");
    info(lowF);

    removeListNode(open, &openLength, lowF);
    printf("\nRemoved lowF from open list.\n");
    nodesInfo(open);

    return 0;
}

// Copies data from Node orig into parent of Node copy
void setParent(Node *copy, Node orig) {
    if(copy->parent == NULL) {
        copy->parent = (Node*) malloc(sizeof(Node));
    }
    copy->parent->exists = orig.exists;
    copy->parent->x = orig.x;
    copy->parent->y = orig.y;
    copy->parent->g = orig.g;
    copy->parent->h = orig.h;
    copy->parent->f = orig.f;
    copy->parent->id = orig.id;
}

void addListNode(Node list[], int* length, Node node) {
	list[node.x] = node;
	list[node.x].exists = 1;
	list[node.x].id = (*length);
	(*length)++;
}

void removeListNode(Node list[], int *length, Node node) {
	int h;

	// If node exists
	if(node.exists == 1) {
		// If node is not the last in list
		if(node.id != (*length)-1) {
			// Iterates through every node in list
			for(h = 0; h < size; h++) {
				// If node has id greater than one being removed
				if(list[h].id > node.id) {
					// Decrement id
					list[h].id--;
				}
			}
		}
		// Clear values on the node
		clearNode(&list[node.x]);

        // Free parent from memory
        free(list[node.x].parent);

		// Decrement length
		(*length)--;
	}
}

// Arranges original opens with a valid id number
// from low to high, into 1D array to be sorted
void arrangeID(Node arrange[], Node original[], int length) {
	int a, h;

	for(a = 0; a < length; a++) {
		for(h = 0; h < size; h++) {
			if(original[h].id == a) {
				arrange[a] = original[h];
                break;
			}
		}
	}
}

Node lowestF(Node open[], int length) {
	// Copies open opens to new array for sorting
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
    printf("SORT\n");
    nodesInfo(sort);
	return sort[0];
}

// Finds open with specific id number index and returns it
Node getNode(Node list[], int findIndex) {
	int h;

	for(h = 0; h < size; h++) {
		if(list[h].id == findIndex) {
            return list[h];
		}
	}
	Node fail;
	fail.exists = 0;
	fail.id = -1;
    return fail;
}

// Initializes all opens to default values
void initNodes(Node node[]) {
	int h;
	for(h = 0; h < size; h++) {
		clearNode(&node[h]);
		node[h].x = h;
		node[h].y = 0;
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

void info(Node node) {
    if(node.parent != NULL) {
        printf("info.. %d, %d. f: %d. id: %d. E: %d ", node.x, node.y, node.f, node.id, node.exists);
        printf("//// Parent: %d, %d. f: %d. id: %d. E: %d\n", node.parent->x, node.parent->y, node.parent->f, node.parent->id, node.parent->exists);
    }
    else {
        printf("info.. %d, %d. f: %d. id: %d. E: %d //// Parent = NULL\n", node.x, node.y, node.f, node.id, node.exists);
    }
}

void nodesInfo(Node nodes[]) {
	int h;
	for(h = 0; h < size; h++) {
        if(nodes[h].exists == 1) {
            info(nodes[h]);
        }
	}
}
