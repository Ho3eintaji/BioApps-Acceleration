#include <stdio.h>
#include <stdlib.h>

#include "core_v_mini_mcu.h"
#include "heepocrates.h"
#include "coubertin.h"


int main (void) {
	
	printf("==== COUBERTIN BASIC OPERATION TEST ====\n");	
	
	coubertin_t coubertin;
	coubertin.base_addr = mmio_region_from_addr((uintptr_t)COUBERTIN_START_ADDRESS);

	uint32_t coubertin_r0;
	uint32_t coubertin_r1;
	uint32_t coubertin_r2;
	uint32_t coubertin_saen;
	
	
	// Write r1 to memory at addr 0
	coubertin_r0 = 0x00000CF0;
	coubertin_r1 = 0xAAAAAAAA;
	coubertin_saen = 1;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	

	// Read memory output of addr 0 to r2
	coubertin_r0 = 0x00000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);

	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==coubertin_r1) {
    	printf("COUBERTIN READ ADDR 0 CORRECT\n");
	} 
	else {
    	printf("COUBERTIN READ ADDR 0 FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}

	// Write r1 to memory at addr 32
	coubertin_r0 = 0x10000CF0;
	coubertin_r1 = 0x00000001;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Read memory output of addr 32 to r2
	coubertin_r0 = 0x10000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);

	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==coubertin_r1) {
    	printf("COUBERTIN READ ADDR 32 CORRECT\n");
	} 
	else {
    	printf("COUBERTIN READ ADDR 32 FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// Set latency to 5
	coubertin_write_latency(&coubertin, 5);
	
	// Make an addition between addr 0 and addr 32
	coubertin_r0 = 0x00083DF8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Write back result of addition in address 64
	coubertin_r0 = 0x200008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Read result of addition
	coubertin_r0 = 0x20000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);

	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0xAAAAAAAB) {
    	printf("COUBERTIN ADDITION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN ADDITION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// Write r1 to memory at addr 1
	coubertin_r0 = 0x00800CF0;
	coubertin_r1 = 0xACF0CA0F;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 33
	coubertin_r0 = 0x10800CF0;
	coubertin_r1 = 0xFFFF0000;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Xor 
	coubertin_r0 = 0x108041F8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write back 
	coubertin_r0 = 0x208008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of XOR
	coubertin_r0 = 0x20800FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x530FCA0F) {
    	printf("COUBERTIN XOR RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN XOR RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// Nor 
	coubertin_r0 = 0x108049F8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write back 
	coubertin_r0 = 0x210008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of NOR
	coubertin_r0 = 0x21000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x000035F0) {
    	printf("COUBERTIN NOR RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN NOR RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// And 
	coubertin_r0 = 0x10804BF8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write back 
	coubertin_r0 = 0x218008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of AND
	coubertin_r0 = 0x21800FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0xACF00000) {
    	printf("COUBERTIN AND RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN AND RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	} 

	return EXIT_SUCCESS;
}
