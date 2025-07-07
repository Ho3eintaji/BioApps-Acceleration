// Copyright 2023 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include "fll.h"
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "data.h"
#include "cgra_bitstream.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "heepocrates_ctrl.h"
#include "cgra.h"

// #define RUN_CGRA
// #define POWER_MEASURE
#define CHECK_RESULTS
#define GPIO_TRIGGER


#define CGRA_COL_INPUT_SIZE 4
#define PIN_TRIGGER     4  //used for trigering and checking on oscilloscope




// // Handler for the CGRA interruption
// void handler_irq_cgra(uint32_t id);

// Gpio
void gpio_output_cfg(gpio_t *gpio, uint32_t pin);
gpio_t gpio;


const uint64_t SYS_FREQ = 80*1000000; //MHz
void fll_cfg(uint64_t freq);

// one dim slot x n input values (data ptrs, constants, ...)
// int32_t cgra_input[CGRA_N_SLOTS][10] __attribute__ ((aligned (4)));
static int32_t cgra_input[CGRA_N_COLS][CGRA_COL_INPUT_SIZE]    __attribute__ ((aligned (4)));
// int8_t cgra_intr_flag;
volatile bool               cgra_intr_flag;
// volatile int32_t cgra_res[OUTPUT_LENGTH] = {0};
// int32_t exp_res[OUTPUT_LENGTH] = {0};
// CGRA variables
static cgra_t               cgra;
static uint8_t              cgra_slot;

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


int32_t R_out[R_ROWS*R_COLS]; 



void __attribute__((noinline, aligned(4))) cpuMatMul(data_t *A, data_t *B, data_t *R_cpu, unsigned int a_rows, unsigned int a_cols, unsigned int b_cols);



int main(int argc, char const *argv[])
{

    // Set app frequency
    fll_cfg(SYS_FREQ);
    gpio_output_cfg(&gpio, PIN_TRIGGER); // GPIO configuration for toggling
    gpio_write(&gpio, PIN_TRIGGER, false);



    unsigned int a_rows = A_ROWS;
    unsigned int a_cols = A_COLS;
    unsigned int b_cols = B_COLS;

    // Init the PLIC
    rv_plic_params.base_addr = mmio_region_from_addr((uintptr_t)PLIC_START_ADDRESS);
    plic_res = dif_plic_init(rv_plic_params, &rv_plic);

    if (plic_res != kDifPlicOk) {
        printf("PLIC init failed\n;");
        return EXIT_FAILURE;
    }

    // Set CGRA priority to 1 (target threshold is by default 0) to trigger an interrupt to the target (the processor)
    plic_res = dif_plic_irq_set_priority(&rv_plic, CGRA_INTR, 1);
    if (plic_res != kDifPlicOk) {
        printf("Set CGRA interrupt priority to 1 failed\n;");
        return EXIT_FAILURE;
    }

    plic_res = dif_plic_irq_set_enabled(&rv_plic, CGRA_INTR, 0, kDifPlicToggleEnabled);
    if (plic_res != kDifPlicOk) {
        printf("Enable CGRA interrupt failed\n;");
        return EXIT_FAILURE;
    }

    // Enable interrupt on processor side
    // Enable global interrupt for machine-level interrupts
    CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
    // Set mie.MEIE bit to one to enable machine-level external interrupts
    const uint32_t mask = 1 << 11;//IRQ_EXT_ENABLE_OFFSET;
    CSR_SET_BITS(CSR_REG_MIE, mask);
    cgra_intr_flag = 0;

    heepocrates_ctrl_t heepocrates_ctrl;
    heepocrates_ctrl.base_addr = mmio_region_from_addr((uintptr_t)HEEPOCRATES_CTRL_START_ADDRESS);
    heepocrates_ctrl_cgra_disable(&heepocrates_ctrl, 0);
    

    cgra_intr_flag = 0;



    while(1) {

        #ifdef RUN_CGRA

            // CGRA
            // cgra_cmem_init(cgra_imem_bistream, cgra_kem_bitstream);
            cgra_cmem_init(cgra_imem_bitstream, cgra_kmem_bitstream);
            // cgra.base_addr = mmio_region_from_addr((uintptr_t)OECGRA_CONFIG_REGS_START_ADDRESS);
            cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);
            cgra_slot = cgra_get_slot(&cgra);

            // Col 0: &B[0][0], nItLoopColsC, &A[0][0], &C[0][3]
            cgra_input[0][0] = &B[0];
            cgra_input[0][1] = R_COLS/CGRA_N_ROWS;
            cgra_input[0][2] = &A[0];
            cgra_input[0][3] = &R_out[3];
            // Col 1: &C[1][0], &B[0][1], nItLoopsColsA, &A[1][0]
            cgra_input[1][0] = &R_out[R_COLS];
            cgra_input[1][1] = &B[1];
            cgra_input[1][2] = A_COLS;
            cgra_input[1][3] = &A[A_COLS];
            // Col 2: &A[2][0], &C[2][1], &B[0][2], nItLoopColsC
            cgra_input[2][0] = &A[2*A_COLS];
            cgra_input[2][1] = &R_out[2*R_COLS+1];
            cgra_input[2][2] = &B[2];
            cgra_input[2][3] = R_COLS/CGRA_N_ROWS;
            // Col 3: nItLoopRowsC, &A[3][0], &C[3][2], &B[0][3], 
            cgra_input[3][0] = R_ROWS/CGRA_N_COLS;
            cgra_input[3][1] = &A[3*A_COLS];
            cgra_input[3][2] = &R_out[3*R_COLS+2];
            cgra_input[3][3] = &B[3];

            // Set CGRA kernel L/S pointers
            for(int col_idx = 0 ; col_idx < CGRA_N_COLS ; col_idx++){
                cgra_set_read_ptr ( &cgra, cgra_slot, (uint32_t) cgra_input[col_idx], col_idx );
            }

            gpio_write(&gpio, PIN_TRIGGER, true);
            #ifdef POWER_MEASURE
            while(1){
            #endif

            // Launch CGRA kernel
            cgra_set_kernel( &cgra, cgra_slot, TRANSFORMER );

            // Wait CGRA is done
            cgra_intr_flag=0;
            while(cgra_intr_flag==0) {
                wait_for_interrupt();
            }
            // Complete the interrupt
            plic_res = dif_plic_irq_complete(&rv_plic, 0, &intr_num);
            if (plic_res != kDifPlicOk || intr_num != CGRA_INTR) {
                printf("CGRA interrupt complete failed\n");
                return EXIT_FAILURE;
            }
            #ifdef POWER_MEASURE
            }
            #endif
            gpio_write(&gpio, PIN_TRIGGER, false);
            // Toggle GPIO to measure power consumption}
        #else
            gpio_write(&gpio, PIN_TRIGGER, true);
            #ifdef POWER_MEASURE 
            while(1){ 
            #endif
            cpuMatMul(A, B, R_out, A_ROWS, A_COLS, B_COLS);
            #ifdef POWER_MEASURE
            }
            #endif
            gpio_write(&gpio, PIN_TRIGGER, false);
        #endif


        #ifdef CHECK_RESULTS
            // check carus, oe-cgra, and cput results to be the same as the golden result
            for (unsigned int i = 0; i < R_ROWS; i++) {
                for (unsigned int j = 0; j < R_COLS; j++) {
                    if (R_out[i*R_COLS+j] != R[i*R_COLS+j]) {
                        printf("CGRA|gold R[%u,%u]: %x %x\n", i, j, R_out[i*R_COLS+j], R[i*R_COLS+j]);
                        // return -1;
                    }
                }
            }
            printf("Results are correct!\n");
        #endif

    }
      
    return 0;
}

void fll_cfg(uint64_t freq) {
  uint32_t fll_freq_real;
  fll_t fll;
  soc_ctrl_t soc_ctrl;
  uint32_t fll_status;
  // 2.1 FLL peripheral structure to access the registers
  fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
  fll_init(&fll);
  fll_status = fll_status_get(&fll);
  soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
  //2.4 Set default app frequency
  fll_set_freq(&fll, freq);
  for (int j = 0; j < 10000; j++) {
    asm volatile("nop");
  }
  fll_freq_real = fll_get_freq(&fll);
  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
}

void gpio_output_cfg(gpio_t *gpio, uint32_t pin) {
	gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, gpio);
    gpio_output_set_enabled(gpio, pin, true);
    gpio_write(gpio, pin, false);
}

void cpuMatMul(data_t *A, data_t *B, data_t *R_cpu, unsigned int a_rows, unsigned int a_cols, unsigned int b_cols)
{
    for (unsigned int i = 0; i < a_rows; i++) {
        for (unsigned int j = 0; j < b_cols; j++) {
            R_cpu[i*b_cols+j] = 0;
            for (unsigned int k = 0; k < a_cols; k++) {
                R_cpu[i*b_cols+j] += A[i*a_cols+k] * B[k*b_cols+j];
            }
        }
    }
}
