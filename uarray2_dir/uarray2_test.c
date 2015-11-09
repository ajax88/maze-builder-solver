/* uarray2_test.c
 * Created by Alex Jackson
 * 	11/7/15
 *
 * testing file for the UArray2 abstraction
 */


#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "uarray2.h"

void init(int row, int col, void *val, void *cl);
void print(int row, int col, void *val, void *cl);

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	UArray2_T my_array = uarray2_new(10, 10, sizeof(int));
	
	uarray2_map(my_array, init, NULL);

	int *curr = uarray2_at(my_array, 4, 4);
	*curr = 12;
	uarray2_map(my_array, print, NULL);

	uarray2_free(&my_array);
	return 0;
}

void init(int row, int col, void *val, void *cl)
{
	(void)cl;
	int *ptr = val;
	*ptr = row + col;
	return;
}

void print(int row, int col, void *val, void *cl)
{
	(void)cl;
	printf("val at row %d, col %d: %d\n", row, col, *(int*)val);
}
