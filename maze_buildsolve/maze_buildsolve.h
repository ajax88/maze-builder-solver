/* maze_buildsolve.h
 * Created by Alex Jackson
 * 	11/9/15
 * Interface for the maze building/solving abstraction
 */

#include "../uarray2_dir/uarray2.h"
#include "../stack_dir/stack.h"

#ifndef MAZE_BUILDERSOLVER_INCLUDED
#define MAZE_BUILDERSOLVER_INCLUDED

typedef struct Maze_T *Maze_T;

extern Maze_T get_maze(int height, int width);
extern void solve_maze(Maze_T maze);
extern void print_maze(Maze_T maze);
extern void free_maze(Maze_T *maze);


#endif
