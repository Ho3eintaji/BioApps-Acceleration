#include <stdio.h>
#include <stdlib.h>

#include "heepocrates.h"
#include "core_v_mini_mcu.h"

#include "./blade_basic_op.h"


int main ()
{
	volatile int* ptr_blade = (int*) BLADE_START_ADDRESS ;
	int err = 0 ;

	volatile int result0 = 0 ;
	volatile int result1 = 0 ;
	volatile int result2 = 0 ;
	volatile int result3 = 0 ;
	volatile int result4 = 0 ;
	volatile int result5 = 0 ;
	volatile int result6 = 0 ;
	volatile int result7 = 0 ;
	volatile int result8 = 0 ;
	volatile int result9 = 0 ;
	volatile int result10 = 0 ;
	volatile int result11 = 0 ;
	volatile int result12 = 0 ;
	volatile int result13 = 0 ;
	volatile int result14 = 0 ;
	volatile int result15 = 0 ;
	volatile int result16 = 0 ;
	volatile int result17 = 0 ;
	
	
	// WRITE
	
	write_32bits (ptr_blade, 0, 0, 0x35ca35ca) ;
	write_32bits (ptr_blade, 1, 0, 0x0000007d) ;
	write_32bits (ptr_blade, 2, 0, 0x11111111) ;
	write_32bits (ptr_blade, 3, 0, 0x11111111) ;
	write_32bits (ptr_blade, 4, 0, 0x11111111) ;
	write_32bits (ptr_blade, 5, 0, 0x11111111) ;
	write_32bits (ptr_blade, 6, 0, 0x00000012) ;
	write_32bits (ptr_blade, 7, 0, 0x0000005f) ;
	write_32bits (ptr_blade, 8, 0, 0x00000069) ;
	
	write_32bits (ptr_blade, 128, 0, 0xac5353ac) ;
	write_32bits (ptr_blade, 129, 0, 0x00000293) ;
	write_32bits (ptr_blade, 130, 0, 0x10101010) ;
	write_32bits (ptr_blade, 131, 0, 0x10101000) ;
	write_32bits (ptr_blade, 132, 0, 0x10100010) ;
	write_32bits (ptr_blade, 133, 0, 0x10101010) ;
	write_32bits (ptr_blade, 134, 0, 0x00000030) ;
	write_32bits (ptr_blade, 135, 0, 0x000000a2) ;
	write_32bits (ptr_blade, 136, 0, 0x000000b7) ;

	write_32bits (ptr_blade, 262, 0, 0x00000000) ;
	write_32bits (ptr_blade, 263, 0, 0x00000000) ;
	write_32bits (ptr_blade, 264, 0, 0x00000000) ;
	write_32bits (ptr_blade, 265, 0, 0x00000000) ;
	write_32bits (ptr_blade, 266, 0, 0x00000000) ;
	write_32bits (ptr_blade, 267, 0, 0x00000000) ;
	
	write_32bits (ptr_blade, 384, 0, 0x00000000) ;
	write_32bits (ptr_blade, 385, 0, 0x00000000) ;
	write_32bits (ptr_blade, 386, 0, 0x00000000) ;
	write_32bits (ptr_blade, 387, 0, 0x00000000) ;
	write_32bits (ptr_blade, 388, 0, 0x00000000) ;
	write_32bits (ptr_blade, 389, 0, 0x00000000) ;
	write_32bits (ptr_blade, 390, 0, 0x00000000) ;
	
	// Single IMC
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 1) ;
	
	
	// XOR
	imc (ptr_blade, 1, 0, 128, 256) ;
	
	result0 = read_32bits (ptr_blade, 256, 0) ;
	
	if (result0 != 0x99996666)
	{
		printf ("First IMC (XOR) not correct !\nExpected result: 0x99996666\nBLADE result: 0x%08x\n\n", result0) ;
		err++ ;
	}
	
	
	// Shift
	imc (ptr_blade, 2, 0, 128, 256) ;
	
	result0 = read_32bits (ptr_blade, 256, 0) ;
	
	if (result0 != 0x6b946b94)
	{
		printf ("Second IMC (Shift) not correct !\nExpected result: 0x6b946b94\nBLADE result: 0x%08x\n\n", result0) ; 
		err++ ;
	}
	
	
	// NOR
	imc (ptr_blade, 3, 0, 128, 256) ;
	
	result0 = read_32bits (ptr_blade, 256, 0) ;
	
	if (result0 != 0x42248811)
	{
		printf ("Third IMC (NOR) not correct !\nExpected result: 0x42248811\nBLADE result: 0x%08x\n\n", result0) ; 
		err++ ;
	}
	
	
	// AND
	imc (ptr_blade, 4, 0, 128, 256) ;
	
	result0 = read_32bits (ptr_blade, 256, 0) ;
	
	if (result0 != 0x24421188)
	{
		printf ("Fourth IMC (AND) not correct !\nExpected result: 0x24421188\nBLADE result: 0x%08x\n\n", result0) ; 
		err++ ;
	}
	
	
	// Add
	imc (ptr_blade, 5, 1, 129, 256) ;
	
	result0 = read_32bits (ptr_blade, 256, 0) ;
	
	if (result0 != 0x00000310)
	{
		printf ("Fifth IMC (Add) not correct !\nExpected result: 0x00000310\nBLADE result: 0x%08x\n\n", result0) ; 
		err++ ;
	}
	
	
	
	
	// hardware loop with AND
	hardware_loop_reg (ptr_blade, 1, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 3) ;
	
	imc (ptr_blade, 4, 2, 130, 257) ;
	
	result1 = read_32bits (ptr_blade, 257, 0) ;
	result2 = read_32bits (ptr_blade, 258, 0) ;
	result3 = read_32bits (ptr_blade, 259, 0) ;
	
	if (result1 != 0x10101010)
	{
		printf ("First IMC of the first AND HW loop not correct !\nExpected result: 0x10101010\nBLADE result: 0x%08x\n\n", result1) ; 
		err++ ;
	}
	if (result2 != 0x10101000)
	{
		printf ("Second IMC of the first AND HW loop not correct !\nExpected result: 0x10101000\nBLADE result: 0x%08x\n\n", result2) ; 
		err++ ;
	}
	if (result3 != 0x10100010)
	{
		printf ("Third IMC of the first AND HW loop not correct !\nExpected result: 0x10100010\nBLADE result: 0x%08x\n\n", result3) ; 
		err++ ;
	}
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 2) ;
	hardware_loop_reg (ptr_blade, 2, 3, 1, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 2) ;
	
	imc (ptr_blade, 4, 4, 132, 260) ;
	
	result4 = read_32bits (ptr_blade, 260, 0) ;
	result5 = read_32bits (ptr_blade, 261, 0) ;
	
	if (result4 != 0x10100010)
	{
		printf ("First IMC of the second AND HW loop not correct !\nExpected result: 0x10100010\nBLADE result: 0x%08x\n\n", result4) ; 
		err++ ;
	}
	if (result5 != 0x10101010)
	{
		printf ("Second IMC of the second AND HW loop not correct !\nExpected result: 0x10101010\nBLADE result: 0x%08x\n\n", result5) ; 
		err++ ;
	}
	
	// MULTIPLICATION
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 1) ;
	
	imc (ptr_blade, 6, 6, 134, 262) ;
	
	result6 = read_32bits (ptr_blade, 262, 0) ;
	
	if (result6 != 0x00000360)
	{
		printf ("First multiplication not correct !\nExpected result: 0x00000360\nBLADE result: 0x%08x\n\n", result6) ; 
		err++ ;
	}
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 3) ;
	
	imc (ptr_blade, 6, 6, 134, 263) ;
	
	result7 = read_32bits (ptr_blade, 263, 0) ;
	result8 = read_32bits (ptr_blade, 264, 0) ;
	result9 = read_32bits (ptr_blade, 265, 0) ;
	
	if (result7 != 0x00000360)
	{
		printf ("First multiplication of the first multiplication HW loop not correct !\nExpected result: 0x00000360\nBLADE result: 0x%08x\n\n", result7) ; 
		err++ ;
	}
	if (result8 != 0x00003c1e)
	{
		printf ("Second multiplication of the first multiplication HW loop not correct !\nExpected result: 0x00003c1e\nBLADE result: 0x%08x\n\n", result8) ; 
		err++ ;
	}
	if (result9 != 0x00004b0f)
	{
		printf ("Second multiplication of the first multiplication HW loop not correct !\nExpected result: 0x00004b0f\nBLADE result: 0x%08x\n\n", result9) ; 
		err++ ;
	}

	hardware_loop_reg (ptr_blade, 1, 3, 0, 2) ;
	hardware_loop_reg (ptr_blade, 2, 3, 1, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 2) ;
	
	imc (ptr_blade, 6, 6, 134, 266) ;
	
	result10 = read_32bits (ptr_blade, 266, 0) ;
	result11 = read_32bits (ptr_blade, 267, 0) ;
	
	if (result10 != 0x00000360)
	{
		printf ("First multiplication of the second multiplication HW loop not correct !\nExpected result: 0x00000360\nBLADE result: 0x%08x\n\n", result10) ; 
		err++ ;
	}
	if (result11 != 0x00003c1e)
	{
		printf ("Second multiplication of the second multiplication HW loop not correct !\nExpected result: 0x00003c1e\nBLADE result: 0x%08x\n\n", result11) ; 
		err++ ;
	}
	
	// CONVOLUTION BLOCK
	
	convolution_reg (ptr_blade, 268, 384) ;
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 1) ;
	
	imc (ptr_blade, 7, 0, 128, 385) ;
	
	result12 = read_32bits (ptr_blade, 385, 0) ;
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 3) ;
	
	imc (ptr_blade, 7, 0, 128, 386) ;
	
	result13 = read_32bits (ptr_blade, 386, 0) ;
	result14 = read_32bits (ptr_blade, 387, 0) ;
	result15 = read_32bits (ptr_blade, 388, 0) ;
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 2) ;
	hardware_loop_reg (ptr_blade, 2, 3, 1, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 2) ;
	
	imc (ptr_blade, 7, 0, 128, 389) ;
	
	result16 = read_32bits (ptr_blade, 389, 0) ;
	result17 = read_32bits (ptr_blade, 390, 0) ;
	
	printf ("Program exited with %d errors.\n\n", err) ;
	
	return 0 ;
}
