#include <stdio.h>
#include <stdlib.h>
#include "conv.h"
#include "blade_basic_op.h"

void __attribute__((noinline)) data_placement (int* ptr_blade, short image [1024], short filter [6][25])
{
	for (int sa = 0 ; sa < 14 ; sa++)
	{
		// writing filters
		for (int i = 0 ; i < 25 ; i++)
		{
			write_16bits ((short*) ptr_blade, i, 1, sa, filter [0][i]) ;
			write_16bits ((short*) ptr_blade, i, 0, sa, filter [1][i]) ;
			write_16bits ((short*) ptr_blade, i + 32, 1, sa, filter [2][i]) ;
			write_16bits ((short*) ptr_blade, i + 32, 0, sa, filter [3][i]) ;
			write_16bits ((short*) ptr_blade, i + 64, 1, sa, filter [4][i]) ;
			write_16bits ((short*) ptr_blade, i + 64, 0, sa, filter [5][i]) ;
		}
		// writing input data
		for (int i = 0 ; i < 8 ; i++)
		{
			for (int j = 0 ; j < 18 ; j++)
			{
				if (sa % 2 == 0)
				{
					write_16bits ((short*) ptr_blade, 8 * j + i + 96, 1, sa, image [j + 32 * i + (sa / 2) * 128]) ;
				}
				else
				{
					write_16bits ((short*) ptr_blade, 8 * j + i + 96, 1, sa, image [j + 32 * i + (sa / 2) * 128 + 14]) ;
				}
			}
		}
	}
}

void __attribute__((noinline)) convolution (int* ptr_blade)
{
	int reset1 = 254 ;
	int accumulation = 255 ;
	int reset2 = 256 ;
	
	// Set the accumulation and reset addresses
	for (int sa = 0 ; sa < 16 ; sa++)
	{
		write_32bits (ptr_blade, reset1, sa, 0) ;
		write_32bits (ptr_blade, reset2, sa, 0) ;
	}
	
	convolution_reg (ptr_blade, accumulation, reset2) ;
	
	
	// Initializing memory for the result of the convolution

	hardware_loop_reg (ptr_blade, 1, 3, 0, 1) ;
	hardware_loop_reg (ptr_blade, 2, 3, 0, 168) ;
	hardware_loop_reg (ptr_blade, 3, 3, 0, 168) ;

	imc (ptr_blade, 4, reset1, 344, 344) ;
	
	
	hardware_loop_reg (ptr_blade, 1, 1, 0, 25) ;
	hardware_loop_reg (ptr_blade, 2, 1, 8, 5) ;
	hardware_loop_reg (ptr_blade, 3, 1, 0, 1) ;
	
	for (int kernel = 0 ; kernel < 3 ; kernel++)
	{
		for (int line = 0 ; line < 4 ; line ++)
		{
			for (int column = 0 ; column < 14 ; column++)
			{
				imc (ptr_blade, 7, 32 * kernel, 96 + line + 8 * column, 344 + column + 14 * line + 56 * kernel) ;
			}
		}
	}
}

void __attribute__((noinline)) result_reading (int* ptr_blade, short result_filter [6][28][28])
{
	for (int sa = 0 ; sa < 14 ; sa ++)
	{
		for (int i = 0 ; i < 4 ; i++)
		{
			for (int j = 0 ; j < 14 ; j++)
			{
				if (sa % 2 == 0)
				{
					result_filter [0][i + 4 * (sa / 2)][j] = read_16bits ((short*) ptr_blade, 344 + j + 14 * i, 1, sa) ;
					result_filter [1][i + 4 * (sa / 2)][j] = read_16bits ((short*) ptr_blade, 344 + j + 14 * i, 0, sa) ;
					result_filter [2][i + 4 * (sa / 2)][j] = read_16bits ((short*) ptr_blade, 400 + j + 14 * i, 1, sa) ;
					result_filter [3][i + 4 * (sa / 2)][j] = read_16bits ((short*) ptr_blade, 400 + j + 14 * i, 0, sa) ;
					result_filter [4][i + 4 * (sa / 2)][j] = read_16bits ((short*) ptr_blade, 456 + j + 14 * i, 1, sa) ;
					result_filter [5][i + 4 * (sa / 2)][j] = read_16bits ((short*) ptr_blade, 456 + j + 14 * i, 0, sa) ;
				}
				else
				{
					result_filter [0][i + 4 * (sa / 2)][j + 14] = read_16bits ((short*) ptr_blade, 344 + j + 14 * i, 1, sa) ;
					result_filter [1][i + 4 * (sa / 2)][j + 14] = read_16bits ((short*) ptr_blade, 344 + j + 14 * i, 0, sa) ;
					result_filter [2][i + 4 * (sa / 2)][j + 14] = read_16bits ((short*) ptr_blade, 400 + j + 14 * i, 1, sa) ;
					result_filter [3][i + 4 * (sa / 2)][j + 14] = read_16bits ((short*) ptr_blade, 400 + j + 14 * i, 0, sa) ;
					result_filter [4][i + 4 * (sa / 2)][j + 14] = read_16bits ((short*) ptr_blade, 456 + j + 14 * i, 1, sa) ;
					result_filter [5][i + 4 * (sa / 2)][j + 14] = read_16bits ((short*) ptr_blade, 456 + j + 14 * i, 0, sa) ;
				}
			}
		}
	}
}
