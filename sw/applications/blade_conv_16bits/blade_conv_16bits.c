#include <stdio.h>
#include <stdlib.h>

#include "heepocrates.h"
#include "core_v_mini_mcu.h"

#include "./image.h"
#include "./filters.h"
#include "./result.h"
#include "./conv.h"
#include "./blade_basic_op.h"
#include "csr.h"




short result_filter [6][28][28] ;

int main ()
{
	volatile int* ptr_blade = (int*) BLADE_START_ADDRESS ;
	unsigned int cycles, cycles1, cycles2 ;
	int err = 0 ;
	
	filter_placement(ptr_blade, filter) ;

	CSR_WRITE(CSR_REG_MCYCLE, 0) ;
	input_placement (ptr_blade, image) ;
	CSR_READ(CSR_REG_MCYCLE, &cycles) ;
	CSR_WRITE(CSR_REG_MCYCLE, 0) ;
	convolution (ptr_blade) ;
	CSR_READ(CSR_REG_MCYCLE, &cycles1) ;
	CSR_WRITE(CSR_REG_MCYCLE, 0) ;
	result_reading (ptr_blade, result_filter) ;
	CSR_READ(CSR_REG_MCYCLE, &cycles2) ;
	
	

	printf("program finished in %d cycles\n", cycles);
	printf("program finished in %d cycles\n", cycles1);
	printf("program finished in %d cycles\n", cycles2);
	
	
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

	for (int filter = 0 ; filter < 6 ; filter ++)
	{
		for (int i = 0 ; i < 28 ; i++)
		{
			for (int j = 0 ; j < 28 ; j++)
			{
				if (result [filter][28 * i + j] != result_filter [filter][i][j])
				{
					err++ ;
					printf("coordiante (%d, %d) of filter %d is not correct\n", i, j, filter) ;
				}
			}
		}
	}

	printf("\n\nThe total number of errors in the convolution is %d\n", err);
	
	return 0 ;
}
