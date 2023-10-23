// Copyright 2023 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "power_manager.h"
// #include "matmul_data.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
#include "gpio.h"

gpio_t gpio;

// int output_C[OUTPUT_EXPECTED_C_SIZE];

int main(int argc, char const *argv[])
{
    // Setup FLL
    uint32_t fll_freq;
    fll_t fll;
    fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
    fll_init(&fll);

    // Setup SoC controller
    uint32_t fll_freq_real;
    soc_ctrl_t soc_ctrl;
    soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);

    // Setup GPIO
    gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, &gpio);
    gpio_output_set_enabled(&gpio, 4, true);

     // Change frequency
    fll_set_freq(&fll, 1*1000000);
    fll_freq_real = fll_get_freq(&fll);
    soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);

    


    while(1)
    {
        // printf("Run\n");
        // for(int i=0; i<10000; i++){
        // // for(int i=0; i<10; i++){
        //     asm volatile("nop");}
    }

    return 0;
}