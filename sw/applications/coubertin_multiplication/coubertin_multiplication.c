#include <stdio.h>
#include <stdlib.h>

#include "core_v_mini_mcu.h"
#include "heepocrates.h"
#include "coubertin.h"


int main (void) {
	
	printf("==== COUBERTIN MULTIPLICATION TEST ====\n");	
	
	coubertin_t coubertin;
	coubertin.base_addr = mmio_region_from_addr((uintptr_t)COUBERTIN_START_ADDRESS);

	uint32_t coubertin_r0;
	uint32_t coubertin_r1;
	uint32_t coubertin_r2;
	uint32_t coubertin_saen;

	// Write r1 to memory at addr 0
	coubertin_r0 = 0x00000CF0;
	coubertin_r1 = 0x02080032;
	coubertin_saen = 1;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 1
	coubertin_r0 = 0x00800CF0;
	coubertin_r1 = 0x00006666;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 32
	coubertin_r0 = 0x10000CF0;
	coubertin_r1 = 0x06000000;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 33
	coubertin_r0 = 0x10800CF0;
	coubertin_r1 = 0x00000006;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 34
	coubertin_r0 = 0x11000CF0;
	coubertin_r1 = 0x00050000;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 35
	coubertin_r0 = 0x11800CF0;
	coubertin_r1 = 0x00000005;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 64 to set accumulation address to 0 for multiplication
	coubertin_r0 = 0x20000CF0;
	coubertin_r1 = 0x00000000;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 65 to set accumulation address to 0 for multiplication
	coubertin_r0 = 0x20800CF0;
	coubertin_r1 = 0x00000000;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 66 to set accumulation address to 0 for multiplication
	coubertin_r0 = 0x21000CF0;
	coubertin_r1 = 0x00000000;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 67 to set accumulation address to 0 for multiplication
	coubertin_r0 = 0x21800CF0;
	coubertin_r1 = 0x00000000;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);

	// START 8-BITS MULTIPLICATION
	
	// Load multiplier in specific reg
	coubertin_r0 = 0x100007F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	for (int i = 0 ; i < 7 ; i++)
	{
		// Add & Shift
		coubertin_r0 = 0x200003F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
		
		// Write back in accumulation address
		coubertin_r0 = 0x200008F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
	}
	
	// Final add 
	coubertin_r0 = 0x20000DF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Final write back 
	coubertin_r0 = 0x200008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of multiplication
	coubertin_r0 = 0x20000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x0C30002C) {
    	printf("COUBERTIN 8-BITS MULTIPLICATION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 8-BITS MULTIPLICATION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// START 32-BITS MULTIPLICATION
	
	// Load multiplier in specific reg
	coubertin_r0 = 0x108007F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	for (int i = 0 ; i < 31 ; i++)
	{
		// Add & Shift
		coubertin_r0 = 0x208033F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
		
		// Write back in accumulation address
		coubertin_r0 = 0x208008F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
	}
	
	// Final add 
	coubertin_r0 = 0x20803DF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Final write back 
	coubertin_r0 = 0x208008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of multiplication
	coubertin_r0 = 0x20800FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x0C30012C) {
    	printf("COUBERTIN 32-BITS MULTIPLICATION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 32-BITS MULTIPLICATION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// START 16-BITS MULTIPLICATION
	
	// Load multiplier in specific reg
	coubertin_r0 = 0x110007F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	for (int i = 0 ; i < 15 ; i++)
	{
		// Add & Shift
		coubertin_r0 = 0x210053F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
		
		// Write back in accumulation address
		coubertin_r0 = 0x210008F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
	}
	
	// Final add 
	coubertin_r0 = 0x21005DF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Final write back 
	coubertin_r0 = 0x210008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of multiplication
	coubertin_r0 = 0x21000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x0000FFFE) {
    	printf("COUBERTIN 16-BITS MULTIPLICATION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 16-BITS MULTIPLICATION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// START 32-BITS MULTIPLICATION
	
	// Load multiplier in specific reg
	coubertin_r0 = 0x118007F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	for (int i = 0 ; i < 31 ; i++)
	{
		// Add & Shift
		coubertin_r0 = 0x218073F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
		
		// Write back in accumulation address
		coubertin_r0 = 0x218008F0;
		coubertin_write_r0(&coubertin, coubertin_r0);
		coubertin_write_saen(&coubertin, coubertin_saen);
	}
	
	// Final add 
	coubertin_r0 = 0x21807DF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Final write back 
	coubertin_r0 = 0x218008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of multiplication
	coubertin_r0 = 0x21800FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x0001FFFE) {
    	printf("COUBERTIN 32-BITS MULTIPLICATION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 32-BITS MULTIPLICATION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}

	return EXIT_SUCCESS;
}
