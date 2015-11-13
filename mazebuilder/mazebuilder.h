/* mazebuilder.h
 * Created by Alex Jackson
 * 	11/9/15
 * Interface for the maze building abstraction
 */

#include "../uarray2_dir/uarray2.h"
#include "../stack_dir/stack.h"

#ifndef MAZEBUILDER_INCLUDED
#define MAZEBUILDER_INCLUDED

typedef struct Maze_T {
	UArray2_T data;
	Stack_T paths;
} *Maze_T;



#define WALL 'X'
#define EMPTY ' '
#define START 'S'
#define END 'E'
#define PATH 'P'

enum Directions{ LEFT, DOWN, UP, RIGHT, NONE };

typedef struct Position {
	int row, col; 
} *Position;

extern Maze_T get_maze(int height, int width);
extern void print_maze(Maze_T maze);
extern void free_maze(Maze_T *maze);


#endif
