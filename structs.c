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
