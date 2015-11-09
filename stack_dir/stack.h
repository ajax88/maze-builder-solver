/* stack.h
 * Created by Alex Jackson
 * 	11/9/15
 * Interface for a stack abstraction
 */

#ifndef STACK_INCLUDED
#define STACK_INCLUDED

typedef struct Stack_T *Stack_T;

/* returns an initialized stack w/ size 0 */
extern Stack_T stack_new();

extern void stack_push(Stack_T stk, void *val);
extern void *stack_pop(Stack_T stk);
extern int is_empty(Stack_T stk);

extern void stack_free(Stack_T *stk);

#endif
