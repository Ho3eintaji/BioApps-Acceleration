#include <stdio.h>
#include <stdlib.h>

#include "core_v_mini_mcu.h"
#include "heepocrates.h"
#include "coubertin.h"

// Use PRINTF instead of PRINTF to remove print by default
#ifdef DEBUG
  #define PRINTF(fmt, ...)    printf(fmt, ## __VA_ARGS__)
#else
  #define PRINTF(...)
#endif

int main(void) {

  printf("==== COUBERTIN TEST ====\n");

  coubertin_t coubertin;
  coubertin.base_addr = mmio_region_from_addr((uintptr_t)COUBERTIN_START_ADDRESS);

  uint32_t coubertin_r0;
  uint32_t coubertin_r1;
  uint32_t coubertin_r2;
  uint32_t coubertin_saen;

  // Check the sub-array is working
  coubertin_write_latency(&coubertin, 5);

  // Write r1 to memory
  coubertin_r0 = 0b00000000000000000011110011110000;
  coubertin_r1 = 0xAAAAAAAA;
  coubertin_saen = 1;
  coubertin_write_r0(&coubertin, coubertin_r0);
  coubertin_write_r1(&coubertin, coubertin_r1);
  coubertin_write_saen(&coubertin, coubertin_saen);

  while(coubertin_read_ready(&coubertin)==false){};

  // Read memory output to r2
  coubertin_r0 = 0b00000000000000000011111111110000;
  coubertin_saen = 1;
  coubertin_write_r0(&coubertin, coubertin_r0);
  coubertin_write_saen(&coubertin, coubertin_saen);

  while(coubertin_read_ready(&coubertin)==false){};
  
  coubertin_r2 = coubertin_read_r2(&coubertin);

  if(coubertin_r2==coubertin_r1) {
    printf("COUBERTIN OP CORRECT\n");
  } else {
    printf("COUBERTIN OP FAILED\n");
    PRINTF("r2=%x\n", coubertin_r2);
  }

  return EXIT_SUCCESS;
}
