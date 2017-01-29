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
