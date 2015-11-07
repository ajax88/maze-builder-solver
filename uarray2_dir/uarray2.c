/* uarray2.c
 * Created by Alex Jackson 
 * 	11/7/15
 * Implementation for an unboxed 2-D array
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "uarray2.h"

struct UArray2_T {
	int height, width, size;
	char *elems;
};

extern UArray2_T uarray2_new(int height, int width, int size)
{
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
/*
extern void *uarray2_at(UArray2_T array, int row, int col);
extern void uarray2_map(UArray2_T array, void apply(int row, int col, void *val,
       			                            void *cl), void *cl);
                        
*/
extern void uarray2_free(UArray2_T *array)
{
	assert(array != NULL && *array != NULL);
	free((*array)->elems);
	free(*array);
	return;
}

