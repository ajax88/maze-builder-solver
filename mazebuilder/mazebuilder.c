/* maze_builder.c 
 * Created by Alex Jackson
 * 	11/9/15
 * Implemenation for the maze builing abstraction
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "../uarray2_dir/uarray2.h"
#include "../stack_dir/stack.h"
#include "mazebuilder.h"

#define WALL 'X'
#define EMPTY ' '
#define START 'S'
#define END 'E'
#define PATH 'P'

enum Directions{ LEFT, DOWN, UP, RIGHT };
struct Maze_T {
	UArray2_T data;

};
/********************* HELPER PROTOTYPES **********************/
static void print_apply(int row, int col, void *val, void *cl);
static void maze_init(int row, int col, void *val, void *cl);


/******************* USER/IMPLEMENTATION FUNCTIONS ************/


extern Maze_T get_maze(int height, int width)
{
	Maze_T maze = malloc(sizeof(*maze));
	maze->data = uarray2_new(height, width, sizeof(char));
	uarray2_map(maze->data, maze_init, maze);
	return maze;

}

extern void print_maze(Maze_T maze)
{
	assert(maze != NULL);
	assert(maze->data != NULL);

	uarray2_map(maze->data, print_apply, maze);
	return;
}

extern void free_maze(Maze_T *maze)
{
	assert(maze != NULL && *maze != NULL);
	uarray2_free(&((*maze)->data));
	free(*maze);
	return;
}


/******************** HELPER FUNCTIONS **************************/


static void print_apply(int row, int col, void *val, void *cl)
{
	(void)row;
	printf("%c", *(char *)val);
	UArray2_T data = ((Maze_T)cl)->data;
	if (col == uarray2_width(data) - 1){
		printf("\n");
	}
	return;
}

static void maze_init(int row, int col, void *val, void *cl)
{
	char *curr = val;
	int height = uarray2_height(((Maze_T)cl)->data);
	int width = uarray2_width(((Maze_T)cl)->data);

	if (row == 1 && col == 0){
		*curr = START;
	} else if (row == height - 2 && col == width - 1) {
		*curr = END;
	}
	else if (row == height - 1 || col == width - 1 || 
			row == 0 || col == 0) {
		*curr = WALL;
	}
	else {
		*curr = EMPTY;
	}

	return; 
}


