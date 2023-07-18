// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <stdlib.h>
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "gpio.h"

static power_manager_t power_manager;


#define GPIO_TEST_OUT 0

int main(int argc, char *argv[])
{

    gpio_t gpio;
    gpio_params_t gpio_params;
    gpio_result_t gpio_res;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_res = gpio_init(gpio_params, &gpio);
    gpio_res = gpio_output_set_enabled(&gpio, GPIO_TEST_OUT, true);

    // Setup power_manager
    mmio_region_t power_manager_reg = mmio_region_from_addr(POWER_MANAGER_START_ADDRESS);
    power_manager.base_addr = power_manager_reg;
    power_manager_counters_t power_manager_counters;
    power_manager_counters_t power_manager_counters_cpu;

    int gpio_val=0;
    int sram_off = 0;
    gpio_write(&gpio, GPIO_TEST_OUT, 0);


    if (power_gate_counters_init(&power_manager_counters, 400, 400, 300, 300, 200, 200, 0, 0) != kPowerManagerOk_e)
    {
        return EXIT_FAILURE;
    }


    while(1){  

	    if(gpio_val > 10000) {
		    sram_off = !sram_off;
		    gpio_val = 0;
		    gpio_write(&gpio, GPIO_TEST_OUT, sram_off);
	    }

	    if(sram_off == 0 && gpio_val == 0) {
   		    
   		    printf("Switch Off SRAM\n");
		    // Power-gate ram blocks 2 to 9
		    if (power_gate_ram_block(&power_manager, 2, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 3, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 4, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 5, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 6, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 7, kOff_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
	    }

	    if(sram_off == 1 && gpio_val == 0) {

   		    printf("Switch On SRAM\n");

		    // Power-on ram blocks 2 to 9
		    if (power_gate_ram_block(&power_manager, 2, kOn_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 3, kOn_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 4, kOn_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 5, kOn_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 6, kOn_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
		    if (power_gate_ram_block(&power_manager, 7, kOn_e, &power_manager_counters) != kPowerManagerOk_e)
		    {
			return EXIT_FAILURE;
		    }
	   }
	   
	   gpio_val++;
      }

    return EXIT_SUCCESS;
}
