/* stack.c
 * Created by Alex Jackson
 * 	11/9/15
 * Implementation for a stack abstraction
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "stack.h"

#define INITIAL_SIZE 8  

/* STATIC PRIVATE FUNCTIONS */
static Stack_T expand(Stack_T stk);

/* USER/INTERFACE FUNCTIONS */

struct Stack_T {
	int size, capacity;
	void **elems;
};

/* returns an initialized stack w/ size 0 */
extern Stack_T stack_new()
{
	Stack_T stk = malloc(sizeof(*stk));
	stk->size = 0;
	stk->capacity = INITIAL_SIZE;
	stk->elems = malloc(sizeof(void *) * INITIAL_SIZE);
	return stk;
}

extern void stack_push(Stack_T stk, void *val)
{
	assert(stk != NULL);
	if (stk->size == stk->capacity) {
		stk = expand(stk);
	}
	stk->elems[stk->size] = val;
	stk->size += 1;
	return;
}
extern void *stack_pop(Stack_T stk)
{
	assert(stk != NULL);
	stk->size -= 1;
	return stk->elems[stk->size];
}

extern int is_empty(Stack_T stk)
{
	assert(stk != NULL);
	if (stk->size)
		return 0; /* if not empty, return false */ 
	else 
		return 1;
}

extern void stack_free(Stack_T *stk)
{
	assert(stk != NULL && *stk != NULL);
	free((*stk)->elems);
	free(*stk);
	return;
}

/* STATIC PRIVATE FUNCTIONS */

static Stack_T expand(Stack_T stk)
{
	assert(stk != NULL);
	stk->capacity *= 2;
	stk->elems = realloc(stk->elems, stk->capacity * sizeof(void *));
	return stk;
}

