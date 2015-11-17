/* uarray2.c
 * Created by Alex Jackson 
 * 	11/7/15
 * Implementation for an unboxed 2-D array
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "uarray2.h"

/* For performance tuning, consider declaring char *elems as 
 * char elems[], this will allow me to malloc the size of the struct to be equal
 * to the 3 ints along with all the space needed for the array given to the
 * user, all held within a single struct. This will reduce the level of pointer 
 * chasing by 1 */
struct UArray2_T {
	int height, width, size;
	char *elems; /* We are storing an array of any size, the storing/
			recovering of data will be handled by the following 
			functions, so the user does not need to care about this
			representation */
};

extern UArray2_T uarray2_new(int height, int width, int size)
{
	/* Does not allow for a 2-D array w/ width or height of 0, as it is not 
	 * dynamic */
	assert(height != 0 && width != 0 && size !=0);

	UArray2_T array = malloc(sizeof(*array));
	assert(array != NULL);

	array->height = height;
	array->width = width;
	array->size = size;

	array->elems = calloc(height*width, size);
	assert(array->elems != NULL);
	
	return array;
}

extern int uarray2_height(UArray2_T array)
{
	assert(array != NULL);
	return array->height;
}
extern int uarray2_width(UArray2_T array)
{
	assert(array != NULL);
	return array->width;
}

extern int uarray2_size(UArray2_T array)
{
	assert(array != NULL);
	return array->size;
}

/* It doesn't matter what the data type is, we just need to
 * give the user the proper address of the first byte of whatever it is */
extern void *uarray2_at(UArray2_T array, int row, int col)
{
	assert(array != NULL);
	int index = (array->width * row + col) * array->size;
	return &(array->elems[index]);
}

extern void uarray2_map(UArray2_T array, void apply(int row, int col, void *val,
       			                            void *cl), void *cl)
{
	assert(array != NULL);
	int row, col;
	for (row = 0; row < array->height; row ++){
		for(col = 0; col < array->width; col ++){
			apply(row, col, uarray2_at(array, row, col), cl);
		}
	}
	return;
}                      

extern void uarray2_free(UArray2_T *array)
{
	assert(array != NULL && *array != NULL);
	free((*array)->elems);
	free(*array);
	return;
}
