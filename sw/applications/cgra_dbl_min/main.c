#include <stdio.h>
#include <stdlib.h>

#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "heepocrates.h"
#include "cgra.h"
#include "cgra_bitstream.h"
#include "stimuli.h"

#define OUTPUT_LENGTH 4

// one dim slot x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_N_SLOTS][10] __attribute__ ((aligned (4)));
int8_t cgra_intr_flag;
int32_t cgra_res[OUTPUT_LENGTH] = {0};
int32_t exp_res[OUTPUT_LENGTH] = {0};

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
  cgra_cmem_init(cgra_imem_bistream, cgra_kem_bitstream);
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
  cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);

  exp_res[0] = stimuli[0];
  exp_res[1] = INT32_MAX;
  exp_res[2] = 0;
  exp_res[3] = -1;

  printf("Run double minimum search on cpu...\n");
  for(int32_t i=1; i<INPUT_LENGTH; i++) {
    if (stimuli[i] < exp_res[0]) {
      exp_res[1] = exp_res[0];
      exp_res[0] = stimuli[i] ;
      exp_res[3] = exp_res[2];
      exp_res[2] = i;
    } else if (stimuli[i] < exp_res[1]) {
      exp_res[1] = stimuli[i];
      exp_res[3] = i;
    }
  }
  printf("\rdone\n");

  // Select request slot of CGRA (2 slots)
  uint8_t cgra_slot = cgra_get_slot(&cgra);
  // input data ptr
  cgra_input[cgra_slot][0] = (int32_t)&stimuli[0];
  // input size
  cgra_input[cgra_slot][1] = INPUT_LENGTH-1;

  printf("Run double minimum search on CGRA...\n");
  cgra_perf_cnt_enable(&cgra, 1);
  int8_t column_idx;
  // Set CGRA kernel pointers
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[cgra_slot], column_idx);
  cgra_set_write_ptr(&cgra, cgra_slot, (uint32_t) cgra_res, column_idx);
  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, DBL_MIN_KER_ID);

  // Wait CGRA is done
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }

  // Check the cgra values are correct
  int32_t errors=0;
  for (int i=0; i<OUTPUT_LENGTH; i++) {
    if (cgra_res[i] != exp_res[i]) {
      printf("[%d]: %d != %d\n", i, cgra_res[i], exp_res[i]);
      printf("[%d]: %08x != %08x\n", i, cgra_res[i], exp_res[i]);
      errors++;
    }
  }

  printf("CGRA double minimum check finished with %d errors\n", errors);

  // Performance counter display
  printf("CGRA kernel executed: %d\n", cgra_perf_cnt_get_kernel(&cgra));
  column_idx = 0;
  printf("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  printf("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  column_idx = 1;
  printf("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  printf("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  column_idx = 2;
  printf("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  printf("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));
  column_idx = 3;
  printf("CGRA column %d active cycles: %d\n", column_idx, cgra_perf_cnt_get_col_active(&cgra, column_idx));
  printf("CGRA column %d stall cycles : %d\n", column_idx, cgra_perf_cnt_get_col_stall(&cgra, column_idx));

  return EXIT_SUCCESS;
}
