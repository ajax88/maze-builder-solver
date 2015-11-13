/* maze_builder.c 
 * Created by Alex Jackson
 * 	11/9/15
 * Implemenation for the maze builing abstraction
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "assert.h"
#include "../uarray2_dir/uarray2.h"
#include "../stack_dir/stack.h"
#include "mazebuilder.h"


/********************* HELPER PROTOTYPES **********************/
static void print_apply(int row, int col, void *val, void *cl);
static void maze_init(int row, int col, void *val, void *cl);
static void build_paths(Maze_T maze);
static char get_char(Maze_T maze, Position p, enum Directions dir);
static int path_is_valid(Maze_T maze, Position p, enum Directions dir);
static Position update_pos(Position p, enum Directions dir, int is_popped);
static void clean_up_maze(int row, int col, void *val, void *cl);


/******************* USER/IMPLEMENTATION FUNCTIONS ************/

/* NEEDSWORK, if the while loop runs more than once (i.e a solution was not 
 * possible for the at least the first call to build_paths, then this will
 * result in a memory leak */
extern Maze_T get_maze(int height, int width)
{
	Maze_T maze = malloc(sizeof(*maze));
	maze->data = uarray2_new(height, width, sizeof(char));
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
static void build_paths(Maze_T maze)
{
	int is_popped = 0;
	srand(time(NULL));
	Position curr = malloc(sizeof(*curr));
	curr->row = 1;
	curr->col = 1;
	maze->paths = stack_new();
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
	int width = uarray2_width(maze->data);
	int height = uarray2_height(maze->data);
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
		if (*curr_char == PATH && p->row != height - 2 
				&& p->col != width - 2) /* special case */
			return 0;
		curr_char = uarray2_at(maze->data, p->row + 2, p->col);
		if (*curr_char == PATH && p->row != height - 2 
				&& p->col != width - 2) /* special case */
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
		if (*curr_char == PATH && p->row != height - 2 
				&& p->col != width - 2) /* special case */
			return 0;
		curr_char = uarray2_at(maze->data, p->row - 1, p->col + 1);
		if (*curr_char == PATH)
			return 0;
		curr_char = uarray2_at(maze->data, p->row, p->col + 2);
		if (*curr_char == PATH && p->row != height - 2 
				&& p->col != width - 2) /* special case */
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
extern void solve_maze(Maze_T maze)
{
	assert(maze != NULL);
	Stack_T paths = stack_new();
	int end_found = 0;
	int no_path = 0;
	char *curr_char;
	/* first path pos will be (1,1) */
	Position curr = malloc(sizeof(*curr));
	curr->row = 1;
	curr->col = 1;
	do {
		curr_char = uarray2_at(maze->data, curr->row, curr->col);
		*curr_char = PATH;
		if (!no_path)
			stack_push(paths, curr);
		no_path = 0;
		for (int i = 0; i < 4; i++) {
			if (get_char(maze, curr, i) == END) {
				end_found = 1;
				break;
			}
			if (get_char(maze, curr, i) == EMPTY) {
				curr = update_pos(curr, i);
				break;
			} /* no valid path around all 4 positions */
			if (i == 3) 
				no_path = 1;
		}
		if (no_path) {
			*curr_char = OLD_PATH;
			//free(curr);
			curr = stack_pop(paths);
		}
		if (end_found)
			break;
	} while (!is_empty(paths));
	if (!end_found)
		printf("MAZE HAS NO SOLUTION\n");
	uarray2_map(maze->data, remove_non_path, NULL);
	return;

}

