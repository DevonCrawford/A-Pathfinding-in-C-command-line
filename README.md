# A* Pathfinding in C (command line)

Recreation of my A* pathfinding algorithm except this time in C! Instead of a graphics interface this is meant to run on the command line and analyze a maze given by a .txt file. '#' characters are walls, 'O' characters are open, 'S' is the start and 'G' is the end. You can make any maze which is rectangular and this program will create a new file in the same directory called "nameSolution.txt". I included two test mazes named "maze.txt" and "sampleMaze2.txt". 

I also time the performance to show the efficiency of my algorithm :)

How to run:
- open command line
- go to directory of executable (cd)
- Make sure the maze file is in the same directory as the executable
- type "c-pathfinding maze.txt" to run and analyze "maze.txt" file
- you may analyze any file by typing name of executable SPACE name of mazefile.txt (or directory + name)

![capture](https://cloud.githubusercontent.com/assets/25334129/22401330/d81b8fee-e59c-11e6-921c-477ee9362644.PNG)

Note: If you try to run executable without command line, it will close automatically because no file argument is given
