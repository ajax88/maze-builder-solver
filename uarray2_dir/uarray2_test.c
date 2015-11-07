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


int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	UArray2_T my_array = uarray2_new(10, 10, sizeof(int));
	fprintf(stderr, "height:%d width:%d size:%d\n",uarray2_height(my_array),
			uarray2_width(my_array), uarray2_size(my_array));
	uarray2_free(&my_array);
	return 0;
}
