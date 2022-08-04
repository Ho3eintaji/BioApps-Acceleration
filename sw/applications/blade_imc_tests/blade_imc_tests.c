#include <stdio.h>
#include <stdlib.h>
#include "core_v_mini_mcu.h"

#include "./blade_basic_op.h"


int main ()
{	
	volatile int* ptr_blade = (int*) EXT_SLAVE_START_ADDRESS ;
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
	
	write_32bits (ptr_blade, 0, 0, 0x00000008) ;
	write_32bits (ptr_blade, 1, 0, 0x11111111) ;
	write_32bits (ptr_blade, 2, 0, 0x11111111) ;
	write_32bits (ptr_blade, 3, 0, 0x11111111) ;
	write_32bits (ptr_blade, 4, 0, 0x11111111) ;
	write_32bits (ptr_blade, 5, 0, 0x11111111) ;
	
	
	write_32bits (ptr_blade, 128, 0, 0x000000aa) ;
	write_32bits (ptr_blade, 129, 0, 0x10101010) ;
	write_32bits (ptr_blade, 130, 0, 0x10101010) ;
	write_32bits (ptr_blade, 131, 0, 0x10101010) ;
	write_32bits (ptr_blade, 132, 0, 0x10101010) ;
	write_32bits (ptr_blade, 133, 0, 0x10101010) ;

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
	
	// AND
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 1) ;

	imc (ptr_blade, 4, 0, 128, 256) ;
	
	result0 = read_32bits (ptr_blade, 256, 0) ;
	
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 3) ;
	
	imc (ptr_blade, 4, 1, 129, 257) ;
	
	result1 = read_32bits (ptr_blade, 257, 0) ;
	result2 = read_32bits (ptr_blade, 258, 0) ;
	result3 = read_32bits (ptr_blade, 259, 0) ;
	
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 2) ;
	hardware_loop_reg (ptr_blade, 2, 3, 1, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 2) ;
	
	imc (ptr_blade, 4, 4, 132, 260) ;
	
	result4 = read_32bits (ptr_blade, 260, 0) ;
	result5 = read_32bits (ptr_blade, 261, 0) ;
	
	// MULTIPLICATION
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 1) ;
	
	imc (ptr_blade, 6, 0, 128, 262) ;
	
	result6 = read_32bits (ptr_blade, 262, 0) ;
	
	hardware_loop_reg (ptr_blade, 1, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 3) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 3) ;
	
	imc (ptr_blade, 6, 0, 128, 263) ;
	
	result7 = read_32bits (ptr_blade, 263, 0) ;
	result8 = read_32bits (ptr_blade, 264, 0) ;
	result9 = read_32bits (ptr_blade, 265, 0) ;

	hardware_loop_reg (ptr_blade, 1, 3, 0, 2) ;
	hardware_loop_reg (ptr_blade, 2, 3, 1, 1) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 2) ;
	
	imc (ptr_blade, 6, 0, 128, 266) ;
	
	result10 = read_32bits (ptr_blade, 266, 0) ;
	result11 = read_32bits (ptr_blade, 267, 0) ;
	
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
	
	return 0 ;
}
