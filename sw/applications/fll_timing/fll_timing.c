/*
 * fll_timing.c
 *
 * Check the timing of the FLL
 * Checking for both modes: FLL closed loop and FLL open loop then closed loop
 * 
 * Author: Hossein Taji
 */

#include <stdio.h>
#include <stdlib.h>
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "heepocrates.h"


fll_t fll;
soc_ctrl_t soc_ctrl;
gpio_t gpio;
uint32_t freq1 = 10*1000000; //MHz
uint32_t freq2 = 20*1000000; //MHz

#define PIN_TRIGGER     1  //used for trigering and checking on oscilloscope

void fll_initialize(void);
void gpio_initialize(void);
// uint32_t fll_set_freq_closed_loop_freq1(uint32_t freq);
uint32_t fll_set_freq_closed_loop(uint32_t freq, char *mode);

int main(int argc, char *argv[])
{
  fll_initialize();
  printf("FLL initialized\n");
  gpio_initialize();
  gpio_output_set_enabled(&gpio, PIN_TRIGGER, true);
  gpio_write(&gpio, PIN_TRIGGER, false);
  printf("GPIO initialized\n");

  uint32_t freq1_real = fll_set_freq_closed_loop(freq1, "no_msrs");
  printf("requested freq: %d KHz, and real freq: %d KHz\n", freq1/1000, freq1_real/1000);
  
  uint32_t freq2_real = fll_set_freq_closed_loop(freq2, "msr_total");
  printf("requested freq: %d KHz, and real freq: %d KHz\n", freq2/1000, freq2_real/1000);


  // while(1);

  return EXIT_SUCCESS;
}

void fll_initialize() {
  uint32_t fll_freq_real;
  uint32_t fll_status;
  fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
  fll_init(&fll);
  soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
  // fll_status = fll_status_get(&fll);
}

// uint32_t fll_set_freq_closed_loop(uint32_t freq) {
//   fll_set_freq(&fll, freq);
//   uint32_t fll_freq_real = fll_get_freq(&fll);
//   soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
//   return fll_freq_real;
// }

uint32_t fll_set_freq_closed_loop(uint32_t freq, char *mode) {
  uint32_t fll_freq_real;
  if (mode == "msr_total") {
    gpio_write(&gpio, PIN_TRIGGER, true);
    fll_set_freq(&fll, freq);
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
    gpio_write(&gpio, PIN_TRIGGER, false);
  } else if (mode == "msr_set_fll"){
    gpio_write(&gpio, PIN_TRIGGER, true);
    fll_set_freq(&fll, freq);
    gpio_write(&gpio, PIN_TRIGGER, false);
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
  } else if (mode == "msr_rd_freq"){
    fll_set_freq(&fll, freq);
    gpio_write(&gpio, PIN_TRIGGER, true);
    fll_freq_real = fll_get_freq(&fll);
    gpio_write(&gpio, PIN_TRIGGER, false);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
  } else if (mode == "msr_set_soc"){
    fll_set_freq(&fll, freq);
    fll_freq_real = fll_get_freq(&fll);
    gpio_write(&gpio, PIN_TRIGGER, true);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
    gpio_write(&gpio, PIN_TRIGGER, false);
  } else if (mode == "msr_rd_and_set_soc"){
    fll_set_freq(&fll, freq);
    gpio_write(&gpio, PIN_TRIGGER, true);
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
    gpio_write(&gpio, PIN_TRIGGER, false);
  }
  else {
    fll_set_freq(&fll, freq);
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
  }
  // // gpio_write(&gpio, PIN_TRIGGER, false);
  // // gpio_write(&gpio, PIN_TRIGGER, true);
  // fll_set_freq(&fll, freq);
  // // gpio_write(&gpio, PIN_TRIGGER, false);
  
  // uint32_t fll_freq_real = fll_get_freq(&fll);
  // gpio_write(&gpio, PIN_TRIGGER, false);
  // // gpio_write(&gpio, PIN_TRIGGER, true);
  // soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
  // // gpio_write(&gpio, PIN_TRIGGER, false);
  return fll_freq_real;
}

void gpio_initialize() {
  gpio_params_t gpio_params;
  gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
  gpio_init(gpio_params, &gpio);
}