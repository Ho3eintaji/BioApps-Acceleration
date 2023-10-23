// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <stdlib.h>
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_timer.h"
#include "power_manager.h"
#include "soc_ctrl.h"
#include "heepocrates.h"
#include "fll.h"
#include "gpio.h"





#define SINGLE_FREQ 100
#define PWR_GATE_PERIPH
#define PWR_GATE_4_RAM_BANKS

static power_manager_t power_manager;
gpio_t gpio;

int main(int argc, char *argv[])
{

    // Setup GPIO
    gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, &gpio);
    gpio_output_set_enabled(&gpio, 4, true);
    gpio_write(&gpio, 4, false);




    // Setup power_manager
    mmio_region_t power_manager_reg = mmio_region_from_addr(POWER_MANAGER_START_ADDRESS);
    power_manager.base_addr = power_manager_reg;
    power_manager_counters_t power_manager_counters_cpu;

    // Setup FLL
    uint32_t fll_freq;
    fll_t fll;
    fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
    fll_init(&fll);

    // Setup SoC controller
    uint32_t fll_freq_real;
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);

    // Change frequency
    fll_set_freq(&fll, 150*1000000);
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);

    // while(1);

    for(int i=0; i<100000; i++)
        asm volatile("nop"); 

    printf("Hi\n");




  //   // change frequency in the level of deep sleep
  //   fll_conf1_reg_t fll_conf1 = fll_conf1_get(&fll);
  //   fll_conf1.op_mode = 0;
  //   uint32_t dco_inp = 170;
  // const uint32_t config3 = fll_create_config_1((fll_conf1_reg_t){
  //     .mult_factor = fll_conf1.mult_factor,
  //     .dco_input   = dco_inp,
  //     .clk_div     = 0x2,
  //     .lock_enable = fll_conf1.lock_enable,
  //     .op_mode     = fll_conf1.op_mode
  //   });
  //     fll_conf1_set(&fll, config3);
  //     for(int i=0; i<1000; i++)
  //           asm volatile("nop");
  //     fll_freq = fll_get_freq(&fll);
  //     soc_ctrl_set_frequency(&soc_ctrl, fll_freq);


    // gpio_write(&gpio, 4, true);
    // while(1);
    uint32_t isoff = 0;

  

#ifdef PWR_GATE_PERIPH
    // Power-gate peripheral domain
    power_manager_counters_t power_manager_counters_periph;

    // isoff = periph_power_domain_is_off(&power_manager_reg);
    // printf ("Is it off: %d\n", isoff);

    power_gate_counters_init(&power_manager_counters_periph, 0, 0, 0, 0, 0, 0, 0, 0);
    // gpio_write(&gpio, 4, true);
    power_gate_periph(&power_manager, kOff_e, &power_manager_counters_periph);
    

    while( periph_power_domain_is_off(&power_manager_reg) != 0);
    // gpio_write(&gpio, 4, false);
    // printf ("Is it off: %d\n", isoff);
    // gpio_write(&gpio, 4, false);

    gpio_write(&gpio, 4, true);

    power_gate_periph(&power_manager, kOn_e, &power_manager_counters_periph);
    while(periph_power_domain_is_off(&power_manager_reg));
    // printf ("Is it off: %d\n", isoff);
    gpio_write(&gpio, 4, false);
#endif

    // gpio_write(&gpio, 4, false);
    // isoff = periph_power_domain_is_off(&power_manager_reg);
    // // printf ("Is it off: %d\n", isoff);

    while(1){
      for(int i=0; i<100000; i++)
        asm volatile("nop"); 
    }

    


// #ifdef PWR_GATE_4_RAM_BANKS
//     // Power-gate 4 memory banks (4 to 7)
//     power_manager_counters_t power_manager_counters_ram;
//     power_gate_counters_init(&power_manager_counters_ram, 0, 0, 0, 0, 0, 0, 0, 0);
//     power_gate_ram_block(&power_manager, 4, kOff_e, &power_manager_counters_ram);
//     power_gate_ram_block(&power_manager, 5, kOff_e, &power_manager_counters_ram);
//     power_gate_ram_block(&power_manager, 6, kOff_e, &power_manager_counters_ram);
//     power_gate_ram_block(&power_manager, 7, kOff_e, &power_manager_counters_ram);
// #endif

    

     // gpio_write(&gpio, 4, false);

     // while(1);

    // Power-gate core
    power_gate_counters_init(&power_manager_counters_cpu, 40, 40, 30, 30, 20, 20, 0, 0);
    CSR_CLEAR_BITS(CSR_REG_MSTATUS, 0x8);
    power_gate_core(&power_manager, kTimer_0_pm_e, &power_manager_counters_cpu);
    CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
    gpio_write(&gpio, 4, false);

    return EXIT_SUCCESS;
}