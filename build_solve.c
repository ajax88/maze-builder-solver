/* build_solve.c 
 * Created by Alex Jackson
 * 	11/9/15
 * Program that utilizes both the maze building and maze solving 
 * abstractions to demonstrate the capabilities given to the user
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "mazebuilder/mazebuilder.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	Maze_T maze = get_maze(80, 100);
	print_maze(maze);
	free_maze(&maze);
	return 0;
}
