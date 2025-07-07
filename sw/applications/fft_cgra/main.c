#include <stdio.h>
#include <stdlib.h>

#include "heepocrates.h"
#include "core_v_mini_mcu.h"
// #include "power_manager.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "heepocrates_ctrl.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "cgra.h"

#include "cgra_bitstream.h"
#include "fxp.h"
#include "defines.h"
#include "fft_data.h"

#define RUN_CGRA
// #define POWER_MEASURE
#define CHECK_RESULTS
#define GPIO_TRIGGER

#define CGRA_COL_INPUT_SIZE 4
#define PIN_TRIGGER     4  //used for trigering and checking on oscilloscope

// #include "timer_sdk.h"

#ifdef CPLX_FFT
  #if FFT_SIZE==512
    #include "fft_factors_512_32b_int.h"
  #endif
  #if FFT_SIZE==1024
    #include "fft_factors_1024_32b_int.h"
  #endif
  #if FFT_SIZE==2048
    #include "fft_factors_2048_32b_int.h"
  #endif
#endif // CPLX_FFT
#ifdef REAL_FFT
  #if FFT_SIZE==512
    #include "fft_factors_256_32b_int.h"
  #endif
  #if FFT_SIZE==1024
    #include "fft_factors_512_32b_int.h"
  #endif
  #if FFT_SIZE==2048
    #include "fft_factors_1024_32b_int.h"
  #endif
#endif // REAL_FFT

// Gpio
void gpio_output_cfg(gpio_t *gpio, uint32_t pin);
gpio_t gpio;


const uint64_t SYS_FREQ = 80*1000000; //MHz
void fll_cfg(uint64_t freq);

// one dim per core x n input values (data ptrs, constants, ...)
int32_t cgra_input[CGRA_N_COLS][CGRA_N_SLOTS][10] __attribute__ ((aligned (4))) = { 0 };
int8_t cgra_intr_flag;
static cgra_t               cgra;
static uint8_t              cgra_slot;

// Interrupt controller variables
dif_plic_params_t rv_plic_params;
dif_plic_t rv_plic;
dif_plic_result_t plic_res;
dif_plic_irq_id_t intr_num;



// Interrupt controller variables
void handler_irq_cgra(uint32_t id) {
    cgra_intr_flag = 1;
}

/* --------------------------------------------------------------------------
 *                     Functions declaration
 * --------------------------------------------------------------------------*/
uint16_t ReverseBits (uint16_t index, uint16_t numBits);
uint16_t NumberOfBitsNeeded (uint16_t powerOfTwo);
void cpu_fft_radix2(fxp *real, fxp *imag, int n);

/* --------------------------------------------------------------------------
 *                     Global variables
 * --------------------------------------------------------------------------*/

// FFT radix-2 variables
fxp RealOut_fft0_fxp[FFT_SIZE] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };
fxp ImagOut_fft0_fxp[FFT_SIZE] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };

#ifdef CGRA_100_PERCENT
  fxp RealOut_fft1_fxp[FFT_SIZE] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };
  fxp ImagOut_fft1_fxp[FFT_SIZE] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };
#endif

fxp RealOut_fxp_exp[FFT_SIZE] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };
fxp ImagOut_fxp_exp[FFT_SIZE] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };

#ifdef REAL_FFT
  fxp re_tmp[FFT_SIZE/2+1] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };
  fxp im_tmp[FFT_SIZE/2+1] __attribute__ ((aligned (4))) __attribute__((section(".xheep_data_interleaved"))) = { 0 };
#endif // REAL_FFT




/* --------------------------------------------------------------------------
 *                     main
 * --------------------------------------------------------------------------*/
int main(void) {

  // Set app frequency
  fll_cfg(SYS_FREQ);
  gpio_output_cfg(&gpio, PIN_TRIGGER); // GPIO configuration for toggling
  gpio_write(&gpio, PIN_TRIGGER, false);


  // timer_cycles_init();
  // timer_start();
  // uint32_t t1, t2, t1_total, t2_total;


  PRINTF("Init CGRA context memory...\n");
  // t1 = timer_get_cycles();
  cgra_cmem_init(cgra_imem_bitstream, cgra_kmem_bitstream);

  // uint32_t t_cgra_init = timer_get_cycles() - t1;
  // #ifdef PRINT_TIME
  //   PRINTF("Time to init CGRA context memory: %d\n", t2 - t1);
  // #endif
  // PRINTF("\rdone\n");

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


  // cgra.base_addr = mmio_region_from_addr((uintptr_t)OECGRA_CONFIG_REGS_START_ADDRESS);
  cgra.base_addr = mmio_region_from_addr((uintptr_t)CGRA_PERIPH_START_ADDRESS);

  while(1){
  while(1){

  //////////////////////////////////////////////////////////
  //
  // COMPLEX FFT radix-2 (Butterfy) implementation
  //
  //////////////////////////////////////////////////////////
#ifdef CPLX_FFT

  // t1_total = timer_get_cycles();
  // t1 = timer_get_cycles();

  PRINTF("Run CGRA FFT on %d points...\n", FFT_SIZE);


  
  cgra_perf_cnt_enable(&cgra, 1);
  uint16_t numBits = NumberOfBitsNeeded ( FFT_SIZE );
  int8_t column_idx;



  // STEP 1: bit reverse
  // PRINTF("Run input bit reverse reordering on %d points on CGRA...\n", FFT_SIZE);

  // Select request slot of CGRA (2 slots)
  // uint32_t cgra_slot = cgra_get_slot(&cgra);
  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[0]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE/2; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = 0; // idx start


    break;
    
  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);

        // break;

  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[FFT_SIZE/2+1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[FFT_SIZE/2]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = FFT_SIZE/2; // idx start

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);

  // uint32_t t_bitrev_launch = timer_get_cycles() - t1;
  

#ifdef CGRA_100_PERCENT
  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[0]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE/2; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = 0; // idx start

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);

  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&input_signal[FFT_SIZE/2+1]; // imaginary part is given second
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&input_signal[FFT_SIZE/2]; // imaginary part is given first
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE; // idx end
  cgra_input[column_idx][cgra_slot][3] = (int32_t)numBits;
  cgra_input[column_idx][cgra_slot][4] = (int32_t)&ImagOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][5] = (int32_t)&RealOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][6] = FFT_SIZE/2; // idx start

  // Launch CGRA kernel
  cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_BITREV_ID);
#endif // CGRA_100_PERCENT

// t1 = timer_get_cycles();

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  // uint32_t t_bitrev_exe = timer_get_cycles() - t1;

  // Step 2: complex-valued FFT computation
  // PRINTF("Run a complex FFT of %d points on CGRA...\n", FFT_SIZE);

  // t1 = timer_get_cycles();

  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&RealOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&f_real[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE;

  column_idx = 1;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&f_imag[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&ImagOut_fft0_fxp[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)numBits;

  // Launch CGRA kernel
  #ifdef CGRA_FFT_FOREVER
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_FOREVER_ID);
  #else
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_ID);
  #endif

  // uint32_t t_fft_launch = timer_get_cycles() - t1;

#ifdef CGRA_100_PERCENT
  cgra_slot = cgra_get_slot(&cgra);
  column_idx = 0;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&RealOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&f_real[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)FFT_SIZE;

  column_idx = 1;
  cgra_set_read_ptr(&cgra, cgra_slot, (uint32_t) cgra_input[column_idx][cgra_slot], column_idx);

  // input data ptr column 0
  cgra_input[column_idx][cgra_slot][0] = (int32_t)&f_imag[0];
  cgra_input[column_idx][cgra_slot][1] = (int32_t)&ImagOut_fft1_fxp[0];
  cgra_input[column_idx][cgra_slot][2] = (int32_t)numBits;

  // Launch CGRA kernel
  #ifdef CGRA_FFT_FOREVER
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_FOREVER_ID);
  #else
    cgra_set_kernel(&cgra, cgra_slot, CGRA_FTT_CPLX_ID);
  #endif
#endif // CGRA_100_PERCENT

// t1 = timer_get_cycles();

  // Wait CGRA is done
  cgra_intr_flag=0;
  while(cgra_intr_flag==0) {
    wait_for_interrupt();
  }
  // uint32_t t_fft_exe = timer_get_cycles() - t1;
  // // Complete the interrupt
  // plic_res = dif_plic_irq_complete(&rv_plic, 0, &intr_num);
  // if (plic_res != kDifPlicOk || intr_num != CGRA_INTR) {
  //   printf("CGRA interrupt complete failed\n");
  //   return EXIT_FAILURE;
  // }
#endif // CPLX_FFT

// uint32_t t_fft_total = timer_get_cycles() - t1_total;

// #ifdef PRINT_TIME
//   PRINTF("Time to init bitsream: %d\n", t_cgra_init);
//   PRINTF("====================================\n");
//   PRINTF("Time to run complex FFT on CGRA: %d\n", t_fft_total);
//   PRINTF("  - bit reverse launch: %d\n", t_bitrev_launch);
//   PRINTF("  - bit reverse execution: %d\n", t_bitrev_exe);
//   PRINTF("  - FFT launch: %d\n", t_fft_launch);
//   PRINTF("  - FFT execution: %d\n", t_fft_exe);
//   PRINTF("====================================\n");
// #endif // PRINT_TIME





#ifdef REAL_FFT
  printf("REAL FFT KERNEL DEPRECATED FOR CURRENT CGRA ARCHITECTURE")
#endif // REAL_FFT



#ifdef CHECK_ERRORS

  int32_t errors=0;
  for (int i=0; i<FFT_SIZE; i++) {
    if(RealOut_fft0_fxp[i] != exp_output_real[i] ||
        ImagOut_fft0_fxp[i] != exp_output_imag[i]) {
          printf("Real[%d] (CGRA/expected) %08x != %08x)\n", i, RealOut_fft0_fxp[i], exp_output_real[i]);
          printf("Imag[%d] (CGRA/expected) %08x != %08x)\n", i, ImagOut_fft0_fxp[i], exp_output_imag[i]);
        errors++;
      }
  }

#ifdef CGRA_100_PERCENT
  for (int i=0; i<FFT_SIZE; i++) {
    if(RealOut_fft1_fxp[i] != exp_output_real[i] ||
        ImagOut_fft1_fxp[i] != exp_output_imag[i]) {
          printf("Real[%d] (out/expected) %08x != %08x)\n", i, RealOut_fft1_fxp[i], exp_output_real[i]);
          printf("Imag[%d] (out/expected) %08x != %08x)\n", i, ImagOut_fft1_fxp[i], exp_output_imag[i]);
        errors++;
      }
  }
#endif

  printf("CGRA FFT computation finished with %d errors\n", errors);
#endif // CHECK_ERRORS

}
}

  return EXIT_SUCCESS;
}

uint16_t ReverseBits (uint16_t index, uint16_t numBits)
{
  uint16_t i, rev;

  for (i=rev=0; i<numBits; i++) {
    rev = (rev << 1) | (index & 1);
    index >>= 1;
  }

  return rev;
}

uint16_t NumberOfBitsNeeded (uint16_t powerOfTwo)
{
  uint16_t i;

  if (powerOfTwo < 2) {
   return 0; // should not happen
  }

  for (i=0;; i++) {
    if (powerOfTwo & (1 << i))
      return i;
  }
}


// --- CPU FFT Implementation ---
void cpu_fft_radix2(fxp *real, fxp *imag, int n) {
  if (n < 2) {
      // Base case: nothing to do for n = 1
      return;
  }

  int numBits = NumberOfBitsNeeded(n);

  // Bit-reversal permutation
  for (int i = 0; i < n; i++) {
      int j = ReverseBits(i, numBits);
      if (j > i) {
          // Swap real and imaginary parts
          fxp temp_real = real[i];
          fxp temp_imag = imag[i];
          real[i] = real[j];
          imag[i] = imag[j];
          real[j] = temp_real;
          imag[j] = temp_imag;
      }
  }

  // Cooley-Tukey algorithm
  for (int s = 1; s <= numBits; s++) {
      int m = 1 << s;       // Butterfly size (2, 4, 8, ..., n)
      int half_m = m >> 1;  // m/2

      for (int k = 0; k < n; k += m) {
          for (int j = 0; j < half_m; j++) {
              // Calculate twiddle factor index (using lookup table)
              int twiddle_index = j * (FFT_SIZE / m);

              fxp w_real, w_imag;

              //important part for handling 512, 1024, ... FFT sizes
              if (twiddle_index < FFT_SIZE / 2) {
                  w_real = f_real[twiddle_index];
                  w_imag = f_imag[twiddle_index];
              }
              else
              {
                  w_real = f_real[twiddle_index - FFT_SIZE / 2];
                  w_imag = -f_imag[twiddle_index - FFT_SIZE / 2];
              }

              // Butterfly operation
              fxp t_real = fxp_mult(w_real, real[k + j + half_m]) - fxp_mult(w_imag, imag[k + j + half_m]);
              fxp t_imag = fxp_mult(w_real, imag[k + j + half_m]) + fxp_mult(w_imag, real[k + j + half_m]);

              fxp u_real = real[k + j];
              fxp u_imag = imag[k + j];

              real[k + j] = u_real + t_real;
              imag[k + j] = u_imag + t_imag;
              real[k + j + half_m] = u_real - t_real;
              imag[k + j + half_m] = u_imag - t_imag;
          }
      }
  }
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