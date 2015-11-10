/* mazebuilder.h
 * Created by Alex Jackson
 * 	11/9/15
 * Interface for the maze building abstraction
 */

#ifndef MAZEBUILDER_INCLUDED
#define MAZEBUILDER_INCLUDED

typedef struct Maze_T *Maze_T;

extern Maze_T get_maze(int height, int width);
extern void print_maze(Maze_T maze);

#endif
