#include <stdio.h>
#include <stdlib.h>
#include "core_v_mini_mcu.h"

#include "./image.h"
#include "./filters.h"
#include "./conv.h"
#include "./blade_basic_op.h"



short result_filter [6][28][28] ;

int main ()
{	
	volatile int* ptr_blade = (int*) EXT_SLAVE_START_ADDRESS ;

	data_placement (ptr_blade, image, filter) ;
	convolution (ptr_blade) ;
	result_reading (ptr_blade, result_filter) ;
	
	
	#ifdef DEBUG_PRINT
	for (int i = 0 ; i < 6 ; i++)
	{
		printf ("[") ;
		for (int j = 0 ; j < 28 ; j++)
		{
			for (int k = 0 ; k < 28 ; k++)
			{
				printf ("%x, ", result_filter [i][j][k]) ;
			}
		}
		printf ("]\n") ;
	}
	#endif	
	
	
	return 0 ;
}
