/* maze_buildsolve.c 
 * Created by Alex Jackson
 * 	11/9/15
 * Implemenation for the maze building/solving abstraction
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "assert.h"
#include "../uarray2_dir/uarray2.h"
#include "../stack_dir/stack.h"
#include "maze_buildsolve.h"

#define WALL 'X'
#define EMPTY ' '
#define START 'S'
#define END 'E'
#define PATH 'P'
#define OLD_PATH 'B'

struct Maze_T {
	UArray2_T data;
	Stack_T paths;
};

typedef struct Position {
	int row, col; 
} *Position;

enum Directions{ LEFT, DOWN, UP, RIGHT, NONE };

/******************************************************************************
 ************************** HELPER FUNCTION PROTOTYPES ************************
 *****************************************************************************/

static void print_apply(int row, int col, void *val, void *cl);
static void maze_init(int row, int col, void *val, void *cl);
static void build_paths(Maze_T maze);
static char get_char(Maze_T maze, Position p, enum Directions dir);
static int path_is_valid(Maze_T maze, Position p, enum Directions dir);
static Position update_pos(Position p, enum Directions dir, int is_popped);
static void clean_up_maze(int row, int col, void *val, void *cl);
static void rm_old_paths(int row, int col, void *val, void *cl);

/******************************************************************************
 ************************** USER/INTERFACE FUNCTIONS ************************** 
 *****************************************************************************/

extern Maze_T get_maze(int height, int width)
{
	assert(height > 3 && width > 3); /* to ensure that the maze is non-
					  trivial (and that it doesn't break) */
	Maze_T maze = malloc(sizeof(*maze));
	maze->data = uarray2_new(height, width, sizeof(char));
	maze->paths = stack_new();
	do {
		uarray2_map(maze->data, maze_init, maze);
		build_paths(maze);
	} while (*(char *)uarray2_at(maze->data, uarray2_height(maze->data) - 2,
				uarray2_width(maze->data) - 2) != PATH);
	/* this do while loop ensures that a solution is found, there are 
	 * certain special cases that prevent a solution from being 
	 * possible based on how each maze cannot have 4 paths in a 2x2 block */
	uarray2_map(maze->data, clean_up_maze, NULL);
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
	stack_free(&((*maze)->paths));
	free(*maze);
	return;
}

/* NEEDSWORK there is a very rare case where if a path is not found where 
 * the stack bounces back to the first path (at index 1,1), this will pop the 
 * position off of the stack and end the loop early. */
extern void solve_maze(Maze_T maze)
{
	assert(maze != NULL);
	assert(is_empty(maze->paths)); /* the stack of the maze shouldn't have 
					 any elements */
	int end_found = 0;
	char *curr_char;
	/* first path pos will be (1,1) */
	Position curr = malloc(sizeof(*curr));
	curr->row = 1;
	curr->col = 1;
	do {
		curr_char = uarray2_at(maze->data, curr->row, curr->col);
		*curr_char = PATH;
		for (int i = 0; i < 4; i++) {
			if (get_char(maze, curr, i) == END) {
				end_found = 1;
				break;
			}
			if (get_char(maze, curr, i) == EMPTY) {
				stack_push(maze->paths, curr);
				curr = update_pos(curr, i, 0);
				break;
			} /* no valid path around all 4 positions */
			if (i == 3) {
				*curr_char = OLD_PATH;
				free(curr);
				curr = stack_pop(maze->paths);
			}
		}
		if (end_found)
			break;
	} while (!is_empty(maze->paths));
	if (!end_found)
		printf("MAZE HAS NO SOLUTION\n");
	uarray2_map(maze->data, rm_old_paths, NULL);
	/* Clean up data */
	while (!is_empty(maze->paths)) {
		free(curr);
		curr = stack_pop(maze->paths);
	}
	free(curr);
	return;
}

/******************************************************************************
 ******************************** HELPER FUNCTIONS **************************** 
 *****************************************************************************/

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
static void build_paths(Maze_T maze)
{
	int is_popped = 0;
	srand(time(NULL));
	Position curr = malloc(sizeof(*curr));
	curr->row = 1;
	curr->col = 1;
	while(1) {
		int is_stuck = 0;
		char *curr_char = uarray2_at(maze->data, curr->row, curr->col);
		*curr_char = PATH;
		if (!is_popped)
			stack_push(maze->paths, curr);
		int dir = (rand() % 4);
		for (int i = 0; i < 4; i ++){
			if (get_char(maze, curr, dir) == EMPTY && 
					path_is_valid(maze, curr, dir)){
				curr = update_pos(curr, dir, is_popped);
				break;
			}
			if (i == 3) {
				is_stuck = 1;
				break;
			}
			if (dir == 3)
				dir = 0;
			else 
				dir += 1;
		}

		if (is_stuck){
			if (is_popped)
				free(curr);
			if (is_empty(maze->paths))
				break;
			curr = stack_pop(maze->paths);
			is_popped = 1;
			continue;
		}
		is_popped = 0;
	}
}

static char get_char(Maze_T maze, Position p, enum Directions dir)
{
	assert(maze != NULL && p != NULL);
	char  *curr_char;
	if (dir == LEFT) {
		curr_char = uarray2_at(maze->data, p->row, p->col - 1);
		return *curr_char;
	} else if (dir == DOWN){
		curr_char = uarray2_at(maze->data, p->row + 1, p->col);
		return *curr_char;
	} else if (dir == UP) {
		curr_char = uarray2_at(maze->data, p->row - 1, p->col);
		return *curr_char;
	} else if (dir == RIGHT) {
		curr_char = uarray2_at(maze->data, p->row, p->col + 1);
		return *curr_char;
	} else {
		curr_char = uarray2_at(maze->data, p->row, p->col);
		return *curr_char;
	}
}
static int path_is_valid(Maze_T maze, Position p, enum Directions dir)
{
	assert(maze != NULL && p != NULL);
	char *curr_char;
	/* Check all positions around p->row, p->col - 1 except for 
	 * path where you came from */
	if (dir == LEFT){
		curr_char = uarray2_at(maze->data, p->row + 1, p->col - 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row - 1, p->col - 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row, p->col - 2);
		if (*curr_char == PATH)
			return 0;
		return 1;
	} else if (dir == DOWN) {
		curr_char = uarray2_at(maze->data, p->row + 1, p->col - 1);
		if (*curr_char == PATH) 
			return 0;
		curr_char = uarray2_at(maze->data, p->row + 1, p->col + 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row + 2, p->col);
		if (*curr_char == PATH) 
			return 0;
		return 1;
	} else if (dir == UP) {
		curr_char = uarray2_at(maze->data, p->row - 1, p->col - 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row - 1, p->col + 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row - 2, p->col);
		if (*curr_char == PATH)
			return 0;
		return 1;
	} else if (dir == RIGHT) {
		curr_char = uarray2_at(maze->data, p->row + 1, p->col + 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row - 1, p->col + 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row, p->col + 2);
		if (*curr_char == PATH)
			return 0;
		return 1;
	} else {
		return 0;
	}
}

static Position update_pos(Position p, enum Directions dir, int is_popped)
{
	assert(p != NULL);
	Position new_pos = malloc(sizeof(*new_pos));
	new_pos->row = p->row;
	new_pos->col = p->col;
	if (is_popped) {
		free(p);
	}
	if (dir == LEFT)
		new_pos->col -= 1;
	else if (dir == DOWN)
		new_pos->row += 1;
	else if (dir == UP)
		new_pos->row -= 1;
	else if (dir == RIGHT)
		new_pos->col += 1;
	else 
		return new_pos;
	return new_pos;
}

static void clean_up_maze(int row, int col, void *val, void *cl)
{
	(void)row;
	(void)col;
	(void)cl;
	char *curr = val;
	if (*curr == EMPTY)
		*curr = WALL;
	if (*curr == PATH)
		*curr = EMPTY;
	return;
}

static void rm_old_paths(int row, int col, void *val, void *cl)
{
	(void)row;
	(void)col;
	(void)cl;
	char *curr = val;
	if (*curr == OLD_PATH)
		*curr = EMPTY;
	return;
}

#undef WALL 
#undef EMPTY
#undef START
#undef END
#undef PATH
#undef OLD_PATH
