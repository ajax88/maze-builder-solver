
/* build_solve.c 
 * Created by Alex Jackson
 * 	11/9/15
 * Program that utilizes both the maze building and maze solving 
 * abstractions to demonstrate the capabilities given to the user
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "maze_buildsolve/maze_buildsolve.h"

#define MAX_INPUT 50
/* Potential features to add: allow user to pipe in a file from stdin
 * or as an arguement on the command line that is a map to be solved.
 * Allow users to enter width/height on command line */
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	int height, width;
	char *trash;
	char input[MAX_INPUT];
	printf("Enter the height of your maze (must be greater than 3)\n");
	fgets(input, MAX_INPUT, stdin);
	height = strtol(input, &trash, 10);

	printf("Enter the width of your maze (must be greater than 3)\n");
	fgets(input, MAX_INPUT, stdin);
	width = strtol(input, &trash, 10);

	Maze_T maze = get_maze(height, width);
	printf("Unsolved maze: \n");
	print_maze(maze);
	solve_maze(maze);
	printf("Solved maze: \n");
	print_maze(maze);
	free_maze(&maze);
	return 0;
}

