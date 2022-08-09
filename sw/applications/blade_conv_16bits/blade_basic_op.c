#include <stdio.h>
#include <stdlib.h>
#include "blade_basic_op.h"


/*
This function aims to write the configuration registers for blade controller when we want to do harware loops.

-> operand: define the register we want to write (1 for opA register, 2 for opB register and 3 for res register)
-> width: define the size of a word in BLADE (0 for 1 byte, 1 for 2 bytes and 3 for 4 bytes)
-> stride: stride in the address loop (must be included between 0 and 255)
-> length: number of consecutive addresses to perform the operation (must be included between 1 and 255)
*/

void __attribute__ ((noinline)) hardware_loop_reg (int * ptr_blade, int operand, int width, int stride, int length)
{
	if (operand == 1) // opA register
	{
		if (length == 1)
		{
			ptr_blade [8193] = (width << 30) + (stride << 9) + 0x1ff ;
		}
		else
		{
			ptr_blade [8193] = (width << 30) + (stride << 9) + (length - 2) ;
		}
	}
	else if (operand == 2) // opB register
	{
		if (length == 1)
		{
			ptr_blade [8194] = (width << 30) + (stride << 9) + 0x1ff ;
		}
		else
		{
			ptr_blade [8194] = (width << 30) + (stride << 9) + (length - 2) ;
		}
	}
	else if (operand == 3) // res register
	{
		if (length == 1)
		{
			ptr_blade [8195] = (width << 30) + (stride << 9) + 0x1ff ;
		}
		else
		{
			ptr_blade [8195] = (width << 30) + (stride << 9) + (length - 2) ;
		}
	}
}


/*
This function aims to write the configuration registers for blade controller to set the accumulation and reset addresses for the convolution.
*/

void __attribute__ ((noinline)) convolution_reg (int * ptr_blade, int accumulation, int reset)
{
	ptr_blade [8196] = (reset << 9) + accumulation ;
}


int __attribute__ ((noinline)) read_32bits (int * ptr_blade, int word, int sa)
{
	return ptr_blade [512 * sa + word] ;
}


short __attribute__ ((noinline)) read_16bits (short * ptr_blade, int word, int byte, int sa)
{
	return ptr_blade [1024 *sa + 2 * word + byte] ;
}


char __attribute__ ((noinline)) read_8bits (char * ptr_blade, int word, int byte, int sa)
{
	return ptr_blade [2048 * sa + 4 * word + byte] ;
}


void __attribute__ ((noinline)) write_32bits (int * ptr_blade, int word, int sa, int data)
{
	ptr_blade [512 * sa + word] = data ;
}


void __attribute__ ((noinline)) write_16bits (short * ptr_blade, int word, int byte, int sa, short data)
{
	ptr_blade [1024 *sa + 2 * word + byte] = data ;
}


void __attribute__ ((noinline)) write_8bits (char * ptr_blade, int word, int byte, int sa, char data)
{
	ptr_blade [2048 * sa + 4 * word + byte] = data ;
}


void __attribute__ ((noinline)) imc (int * ptr_blade, int operation, int opA, int opB, int res)
{
	ptr_blade [8192] = (operation << 27) + (res << 18) + (opB << 9) + opA ;
}
