////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                 Name Surname - email (affiliation if not ESL)              //
//                                                                            //
// File Name:      hello_cgra.c                                               //
//                                                                            //
// Project Name:   CGRA_ESL                                                   //
//                                                                            //
// Language:       C                                                          //
//                                                                            //
// Description:    CGRA test and example code.                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "cgra.h"

#include "stimuli.h"

// one dim slot x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_N_SLOTS][10] __attribute__ ((aligned (4)));
int8_t cgra_intr_flag;

// Interrupt controller variables
dif_plic_params_t rv_plic_params;
dif_plic_t rv_plic;
dif_plic_result_t plic_res;
dif_plic_irq_id_t intr_num;

void handler_irq_external(void) {
    // Claim/clear interrupt
    plic_res = dif_plic_irq_claim(&rv_plic, 0, &intr_num);
    if (plic_res == kDifPlicOk && intr_num == CGRA_INTR) {
        cgra_intr_flag = 1;
    }
}

int main(void) {

  printf("Init CGRA context memory...\n");
  cgra_cmem_init();
  printf("\rdone\n");

  printf("Init the PLIC...");
  rv_plic_params.base_addr = mmio_region_from_addr((uintptr_t)PLIC_START_ADDRESS);
  plic_res = dif_plic_init(rv_plic_params, &rv_plic);

  if (plic_res == kDifPlicOk) {
    printf("success\n");
  } else {
    printf("fail\n;");
  }

  printf("Set CGRA interrupt priority to 1...");
  // Set dma priority to 1 (target threshold is by default 0) to trigger an interrupt to the target (the processor)
  plic_res = dif_plic_irq_set_priority(&rv_plic, CGRA_INTR, 1);
  if (plic_res == kDifPlicOk) {
    printf("success\n");
  } else {
    printf("fail\n;");
  }

  printf("Enable CGRA interrupt...");
  plic_res = dif_plic_irq_set_enabled(&rv_plic, CGRA_INTR, 0, kDifPlicToggleEnabled);
  if (plic_res == kDifPlicOk) {
    printf("success\n");
  } else {
    printf("fail\n;");
  }

  // Enable interrupt on processor side
  // Enable global interrupt for machine-level interrupts
  CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
  // Set mie.MEIE bit to one to enable machine-level external interrupts
  const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
  CSR_SET_BITS(CSR_REG_MIE, mask);
  cgra_intr_flag = 0;

  cgra_t cgra;
  cgra.base_addr = mmio_region_from_addr((uintptr_t)EXT_PERIPHERAL_START_ADDRESS);

  int32_t kernel_res[4]    = {0, 0, 0, 0};
  int32_t cgra_res[4]      = {0, 0, 0, 0};
  int32_t length           = INPUT_LENGTH;

  kernel_res[0] = stimuli[0];
  kernel_res[1] = INT32_MIN;
  kernel_res[2] = 0;
  kernel_res[3] = -1;

  printf("Run double maximum search on cpu...\n");
  for(int32_t i=1; i<length; i++) {
    if (stimuli[i] > kernel_res[0]) {
      kernel_res[1] = kernel_res[0];
      kernel_res[0] = stimuli[i] ;
      kernel_res[3] = kernel_res[2];
      kernel_res[2] = i;
    } else if (stimuli[i] > kernel_res[1]) {
      kernel_res[1] = stimuli[i];
      kernel_res[3] = i;
    }
  }
  printf("\rdone\n");

  // Select request slot of CGRA (2 slots)
  uint8_t cgra_slot = 0;
  // input data ptr
  cgra_input[cgra_slot][0] = (int32_t)&stimuli[0];
  // input size
  cgra_input[cgra_slot][1] = length-1;

  printf("Run double maximum search on CGRA...\n");
  // Set CGRA kernel pointers
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[0], 0);
  cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res, 0);
  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, DBL_MAX_KER_ID);

  // for (int i=0; i<100; i++) {
  //   asm volatile("nop");
  // }
  // printf("\rdone\n");

  // Wait CGRA is done
  while(cgra_intr_flag==0) {
      wait_for_interrupt();
  }

  // Check the cgra values are correct
  int32_t errors=0;
  for (int i=0; i<4; i++) {
    if (cgra_res[i] != kernel_res[i]) {
      errors++;
    }
  }

  printf("CGRA test finished with %d errors\n", errors);

  return EXIT_SUCCESS;
}
