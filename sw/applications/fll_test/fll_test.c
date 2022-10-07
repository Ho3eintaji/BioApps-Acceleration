#include <stdio.h>
#include <stdlib.h>

#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "heepocrates.h"
#include "fll.h"
#include "soc_ctrl.h"
#include "x-heep.h"

// Choose what to test
#define FLL_DEFAULT_VAL_TEST
#define FLL_OPEN_LOOP_TEST
#define FLL_NORMAL_MODE_TEST

uint32_t fll_init(const fll_t *fll);
static uint32_t fll_get_freq_from_mult_div(uint32_t mult_factor, uint32_t clk_div);

int main(void) {

  uint32_t fll_freq;

  // FLL peripheral structure to access the registers
  fll_t fll;
  fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);

  soc_ctrl_t soc_ctrl;
  soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);

  uint32_t fll_status = fll_status_get(&fll);
  fll_conf1_reg_t fll_conf1 = fll_conf1_get(&fll);
  fll_conf2_reg_t fll_conf2 = fll_conf2_get(&fll);
  fll_integrator_reg_t fll_integrator = fll_integrator_get(&fll);

  fll_freq = fll_get_freq(&fll);
  printf("fll_status : %08x\n", fll_status);
  printf("fll_freq Hz: %d\n", fll_freq);

#ifdef FLL_DEFAULT_VAL_TEST
  #ifdef PRINT_FLL_DEFAULT_VAL_TEST
    printf("================ FLL DEFAULT/RESET VALUES CHECK ================\n");
    printf("fll_conf1.mult_factor (resval=0xC35): %08x\n", fll_conf1.mult_factor);
    printf("fll_conf1.dco_input (resval=0x158): %08x\n", fll_conf1.dco_input);
    printf("fll_conf1.clk_div (resval=0x002): %08x\n", fll_conf1.clk_div);
    printf("fll_conf1.lock_enable (resval=0x001): %08x\n", fll_conf1.lock_enable);
    printf("fll_conf1.op_mode (resval=0x000): %08x\n", fll_conf1.op_mode);
    printf("fll_conf2.loop_gain (resval=0x008): %08x\n", fll_conf2.loop_gain);
    printf("fll_conf2.deassert_cycles (resval=0x010): %08x\n", fll_conf2.deassert_cycles);
    printf("fll_conf2.assert_cycles (resval=0x010): %08x\n", fll_conf2.assert_cycles);
    printf("fll_conf2.lock_tolerance (resval=0x200): %08x\n", fll_conf2.lock_tolerance);
    printf("fll_conf2.clk_sta_mode (resval=0x000): %08x\n", fll_conf2.clk_sta_mode);
    printf("fll_conf2.open_loop_enable (resval=0x000): %08x\n", fll_conf2.open_loop_enable);
    printf("fll_conf2.dithering_enable (resval=0x000): %08x\n", fll_conf2.dithering_enable);
    printf("fll_integrator.frac_part (resval=0x000): %08x\n", fll_integrator.frac_part);
    printf("fll_integrator.int_part (resval=0x158): %08x\n", fll_integrator.int_part);
  #endif

  if (fll_conf1.mult_factor      != 0xC35 ||
      fll_conf1.dco_input        != 0x158 ||
      fll_conf1.clk_div          != 0x002 ||
      fll_conf1.lock_enable      != 0x001 ||
      fll_conf1.op_mode          != 0x000 ||
      fll_conf2.loop_gain        != 0x008 ||
      fll_conf2.deassert_cycles  != 0x010 ||
      fll_conf2.assert_cycles    != 0x010 ||
      fll_conf2.lock_tolerance   != 0x200 ||
      fll_conf2.clk_sta_mode     != 0x000 ||
      fll_conf2.open_loop_enable != 0x000 ||
      fll_conf2.dithering_enable != 0x000 ||
      fll_integrator.frac_part   != 0x000 ||
      fll_integrator.int_part    != 0x158) {
    printf("FLL default values incorrect!\n");
  } else {
    printf("FLL default values correct!\n");
  }
#endif

#ifdef FLL_OPEN_LOOP_TEST
  printf("================ FLL OPEN LOOP / STANDALONE MODE CHECK ================\n");
  // FLL Open loop FLL DCO code transfer function (in typical case)
  // !!! FLL Documentation inverts the formulas for {272:360} and {360:31023} !!!
  // dco_input < 272  : freq = 0.000001
  // dco_input < 360  : freq =  1068.966 - 8.626*dco_input + 0.017612*(dco_input**2.0)
  // dco_input >= 360 : freq = -1586.080 + 5.518*dco_input + -0.001191*(dco_input**2.0)
  // FLL Standalone mode (open loop) DCO input code ramp-down

  uint32_t dco_input_default = fll_conf1.dco_input; // 344 => ~50MHz

  for (int i = 0; i < (dco_input_default-272-4); ++i)
  {
    const uint32_t config1 = fll_create_config_1((fll_conf1_reg_t){
        .mult_factor = fll_conf1.mult_factor,
        .dco_input   = dco_input_default-i,
        .clk_div     = fll_conf1.clk_div,
        .lock_enable = fll_conf1.lock_enable,
        .op_mode     = fll_conf1.op_mode
    });
    fll_conf1_set(&fll, config1);
    fll_status = fll_status_get(&fll);
    // Update frequency in SoC controller otherwise devices using this value (e.g., uart) will not work
    fll_freq = fll_get_freq(&fll); //(fll_status*REFERENCE_CLOCK_Hz)/fll_conf1.clk_div;
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq);
    // DCO input code is filtered so changing it slowly does not immediatly change the frequency
    printf("fll DCO input : %d\n", dco_input_default-i);
    printf("fll_freq Hz   : %d\n", fll_freq);
  }
  printf("FLL open loop mode working if you can read this!\n");
#endif // FLL_OPEN_LOOP_TEST


#ifdef FLL_NORMAL_MODE_TEST
  printf("================ FLL NORMAL/LOCK MODE CHECK ================\n");
  
  uint32_t fll_freq_real;

  fll_freq = fll_init(&fll);

  fll_freq_real = fll_get_freq(&fll);

  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);

  printf("fll_freq request Hz: %d\n", fll_freq);
  printf("fll_freq real Hz   : %d\n", fll_freq_real);

  // Set FLL to 100 MHz
  fll_freq = fll_set_freq(&fll, 100000000);

  fll_freq_real = fll_get_freq(&fll);

  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);

  printf("fll_freq request Hz: %d\n", fll_freq);
  printf("fll_freq real Hz   : %d\n", fll_freq_real);

  printf("FLL normal mode working if you can read this!\n");

#endif // FLL_NORMAL_MODE_TEST

  return EXIT_SUCCESS;
}
