#include <stdio.h>
#include <stdlib.h>

#include "core_v_mini_mcu.h"
#include "heepocrates.h"
#include "coubertin.h"


int main (void) {
	
	printf("==== COUBERTIN ADDITION TEST ====\n");	
	
	coubertin_t coubertin;
	coubertin.base_addr = mmio_region_from_addr((uintptr_t)COUBERTIN_START_ADDRESS);

	uint32_t coubertin_r0;
	uint32_t coubertin_r1;
	uint32_t coubertin_r2;
	uint32_t coubertin_saen;
	
	// Write r1 to memory at addr 0
	coubertin_r0 = 0x00000CF0;
	coubertin_r1 = 0x00993506;
	coubertin_saen = 1;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 1
	coubertin_r0 = 0x00800CF0;
	coubertin_r1 = 0xFFFFFFFF;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 32
	coubertin_r0 = 0x10000CF0;
	coubertin_r1 = 0x017769B7;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write r1 to memory at addr 33
	coubertin_r0 = 0x10800CF0;
	coubertin_r1 = 0x00000001;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_r1(&coubertin, coubertin_r1);
	coubertin_write_saen(&coubertin, coubertin_saen);

	// 8-bits addition
	coubertin_r0 = 0x10000DF8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write back  
	coubertin_r0 = 0x200008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of addition
	coubertin_r0 = 0x20000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x01109EBD) {
    	printf("COUBERTIN 8-BITS ADDITION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 8-BITS ADDITION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// 16-bits addition
	coubertin_r0 = 0x10001DF8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write back  
	coubertin_r0 = 0x208008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of addition
	coubertin_r0 = 0x20800FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x02109EBD) {
    	printf("COUBERTIN 16-BITS ADDITION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 16-BITS ADDITION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}
	
	// 32-bits addition
	coubertin_r0 = 0x10003DF8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write back  
	coubertin_r0 = 0x210008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of addition
	coubertin_r0 = 0x21000FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x02109EBD) {
    	printf("COUBERTIN 32-BITS ADDITION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 32-BITS ADDITION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}

	// 32-bits addition
	coubertin_r0 = 0x10807DF8;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	// Write back  
	coubertin_r0 = 0x218008F0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	
	// Read result of addition
	coubertin_r0 = 0x21800FF0;
	coubertin_write_r0(&coubertin, coubertin_r0);
	coubertin_write_saen(&coubertin, coubertin_saen);
	
	while(coubertin_read_ready(&coubertin)==false){};
	coubertin_r2 = coubertin_read_r2(&coubertin);
	
	if(coubertin_r2==0x00000000) {
    	printf("COUBERTIN 32-BITS ADDITION RESULT CORRECT\n");
	} 
	else {
    	printf("COUBERTIN 32-BITS ADDITION RESULT FAILED\n");
    	printf("r2=%x\n", coubertin_r2);
	}

	return EXIT_SUCCESS;
}
