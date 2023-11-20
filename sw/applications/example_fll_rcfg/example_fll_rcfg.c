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
#include "gpio.h"
#include "power_manager.h"




// Choose what to test
// #define FLL_DEFAULT_VAL_TEST
#define FLL_OPEN_LOOP_TEST
// #define FLL_NORMAL_MODE_TEST
// #define FLL_TWO_STEP_RCFG_TEST

// #define FLL_OPEN_LOOP_VALUES_TEST

static const uint64_t kTickFreqHz = 1000 * 1000; // 1 MHz

static power_manager_t power_manager;

uint32_t fll_init(const fll_t *fll);
static uint32_t fll_get_freq_from_mult_div(uint32_t mult_factor, uint32_t clk_div);
gpio_t gpio;

int main(void) {

  /*
  * ==========================================
  * ============== configuration ============= 
  * ==========================================
  */

  uint32_t fll_freq, fll_freq_real;

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

  // Setup GPIO
    gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, &gpio);
    gpio_output_set_enabled(&gpio, 4, true);

#ifdef FLL_DEFAULT_VAL_TEST
  if (fll_conf1.mult_factor      != 6103  || // reset value 0xC35 is changed by external_crt0.S init to 100 MHz
      fll_conf1.dco_input        != 395   || // reset value 0x158 is changed by external_crt0.S init to 100 MHz
      fll_conf1.clk_div          != 0x002 ||
      fll_conf1.lock_enable      != 0x001 ||
      fll_conf1.op_mode          != 0x001 || // reset value 0x000 is changed by external_crt0.S init to 100 MHz
      fll_conf2.loop_gain        != 0x008 ||
      fll_conf2.deassert_cycles  != 0x010 ||
      fll_conf2.assert_cycles    != 0x006 || // reset value 0x010 is changed by external_crt0.S init to 100 MHz
      fll_conf2.lock_tolerance   != 0x050 || // reset value 0x200 is changed by external_crt0.S init to 100 MHz
      fll_conf2.clk_sta_mode     != 0x000 ||
      fll_conf2.open_loop_enable != 0x000 ||
      fll_conf2.dithering_enable != 0x000 ||
      fll_integrator.frac_part   != 0x058 || // reset value 0x000 is changed by external_crt0.S init to 100 MHz
      fll_integrator.int_part    != 0x189) { // reset value 0x158 is changed by external_crt0.S init to 100 MHz
    
    printf("FLL default values incorrect!\n");
      #ifdef PRINT_FLL_DEFAULT_VAL_TEST
        printf("================ FLL DEFAULT/INIT VALUES CHECK ================\n");
        printf("fll_conf1.mult_factor (initval=0x17d7): %08x\n", fll_conf1.mult_factor);
        printf("fll_conf1.dco_input (initval=0x18b): %08x\n", fll_conf1.dco_input);
        printf("fll_conf1.clk_div (resval=0x002): %08x\n", fll_conf1.clk_div);
        printf("fll_conf1.lock_enable (resval=0x001): %08x\n", fll_conf1.lock_enable);
        printf("fll_conf1.op_mode (initval=0x001): %08x\n", fll_conf1.op_mode);
        printf("fll_conf2.loop_gain (resval=0x008): %08x\n", fll_conf2.loop_gain);
        printf("fll_conf2.deassert_cycles (resval=0x010): %08x\n", fll_conf2.deassert_cycles);
        printf("fll_conf2.assert_cycles (initval=0x006): %08x\n", fll_conf2.assert_cycles);
        printf("fll_conf2.lock_tolerance (initval=0x050): %08x\n", fll_conf2.lock_tolerance);
        printf("fll_conf2.clk_sta_mode (resval=0x000): %08x\n", fll_conf2.clk_sta_mode);
        printf("fll_conf2.open_loop_enable (resval=0x000): %08x\n", fll_conf2.open_loop_enable);
        printf("fll_conf2.dithering_enable (resval=0x000): %08x\n", fll_conf2.dithering_enable);
        printf("fll_integrator.frac_part (initval=0x058): %08x\n", fll_integrator.frac_part);
        printf("fll_integrator.int_part (initval=0x189): %08x\n", fll_integrator.int_part);
      #endif
  } else {
    printf("FLL default/init values correct!\n");
  }
#endif

#ifdef FLL_OPEN_LOOP_TEST
  printf("================ FLL OPEN LOOP / STANDALONE MODE CHECK ================\n");
  uint32_t dco_input_default = 0x158; 

  // Enable open loop mode
  // fll_freq = fll_set_freq(&fll, 50000000);
  fll_conf1.op_mode = 0;
  fll_conf1_set(&fll, fll_conf1.raw);
  fll_conf1.dco_input = dco_input_default;
  fll_conf1_set(&fll, fll_conf1.raw);
  // Small delay to let the FLL settle
  for (int j = 0; j < 1000; j++) {
    asm volatile("nop");
  }
  fll_freq_real = fll_get_freq(&fll);
  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
  printf("OPEN LOOP: fll_freq real Hz = %d\n", fll_freq_real);

  // DCO input code is filtered so the value is incremented by 10 to see a change in frequency
  for (int i = 0; i < (dco_input_default-272-4); i+=10)
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
    // Small delay to let the FLL settle
    for (int j = 0; j < 10000; j++) {
      asm volatile("nop");
    }
    // Update frequency in SoC controller otherwise devices using this value (e.g., uart) will not work
    fll_freq = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq);
    for (int j = 0; j < 10000; j++) {
      asm volatile("nop");
    }
    printf("OPEN LOOP: fll DCO input = %d\n", dco_input_default-i);
    printf("OPEN LOOP: fll_freq Hz   = %d\n", fll_freq);
  }

  uint32_t dco_in = 807;
  dco_in = 170;
  const uint32_t config2 = fll_create_config_1((fll_conf1_reg_t){
      .mult_factor = fll_conf1.mult_factor,
      .dco_input   = dco_in,
      .clk_div     = fll_conf1.clk_div,
      .lock_enable = fll_conf1.lock_enable,
      .op_mode     = fll_conf1.op_mode
    });

    fll_conf1_set(&fll, config2);
    fll_status = fll_status_get(&fll);
    // Small delay to let the FLL settle
    for (int j = 0; j < 10000; j++) {
      asm volatile("nop");
    }
    // Update frequency in SoC controller otherwise devices using this value (e.g., uart) will not work
    fll_freq = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq);
    for (int j = 0; j < 10000; j++) {
      asm volatile("nop");
    }
    printf("OPEN LOOP: DCO input = %d\n", dco_in);
    printf("OPEN LOOP: fll freq   = %d\n", fll_freq);
#endif // FLL_OPEN_LOOP_TEST


#ifdef FLL_NORMAL_MODE_TEST
  printf("================ FLL NORMAL/LOCK MODE CHECK ================\n");
  fll_freq = fll_init(&fll);
  fll_freq_real = fll_get_freq(&fll);
  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
  printf("NORMAL MODE: fll_freq request Hz = %d\n", fll_freq);
  printf("NORMAL MODE: fll_freq real Hz    = %d\n", fll_freq_real);
  // Set FLL to 150 MHz
  fll_freq = fll_set_freq(&fll, 150000000);
  fll_freq_real = fll_get_freq(&fll);
  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
  printf("NORMAL MODE: fll_freq request Hz = %d\n", fll_freq);
  printf("NORMAL MODE: fll_freq real Hz    = %d\n", fll_freq_real);
#endif // FLL_NORMAL_MODE_TEST


#ifdef FLL_TWO_STEP_RCFG_TEST
  printf("================ FLL TWO STEP RCFG TEST ================\n");

  uint32_t dco_inp = 180;
  const uint32_t config3 = fll_create_config_1((fll_conf1_reg_t){
      .mult_factor = fll_conf1.mult_factor,
      .dco_input   = dco_inp,
      .clk_div     = 0x2,
      .lock_enable = fll_conf1.lock_enable,
      .op_mode     = fll_conf1.op_mode
    });

  dco_inp = 807;
  const uint32_t config4 = fll_create_config_1((fll_conf1_reg_t){
      .mult_factor = fll_conf1.mult_factor,
      .dco_input   = dco_inp,
      .clk_div     = 0x2,
      .lock_enable = fll_conf1.lock_enable,
      .op_mode     = fll_conf1.op_mode
    });

  while(1){
    // Change frequency (open loop)
    fll_conf1_set(&fll, config3);
    fll_freq = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq);

    for(int i=0; i<1000; i++)
      asm volatile("nop");

    // gpio_write(&gpio, 4, true);

    // open loop stage
    fll_conf1_set(&fll, config4);
    // feedback stage
    fll_set_freq(&fll, 150*1000000);
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);

    // gpio_write(&gpio, 4, false);

    for(int i=0; i<1000; i++)
        asm volatile("nop");

    printf("Freq    = %d\n", fll_freq_real);
         

#endif // FLL_TWO_STEP_RCFG_TEST


  return EXIT_SUCCESS;
}
