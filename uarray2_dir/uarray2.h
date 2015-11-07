/* uarray.h
 * Created by Alex Jackson
 * 	11/7/15
 * Interface for an unboxed 2-D array
 * Modeled after hanson interfaces/implementations
 */ 

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

typedef struct UArray2_T *UArray2_T;

extern UArray2_T uarray2_new(int height, int width, int size);
extern int uarray2_height(UArray2_T array);
extern int uarray2_width(UArray2_T array);
extern int uarray2_size(UArray2_T array);

extern void *uarray2_at(UArray2_T array, int row, int col);

extern void uarray2_map(UArray2_T array, void apply(int row, int col, void *val,
			void *cl), void *cl);

extern void uarray2_free(UArray2_T *array);


#endif 
