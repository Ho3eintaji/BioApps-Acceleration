// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <stdlib.h>
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "rv_plic.h"
#include "rv_plic_regs.h"
#include "gpio.h"

#define GPIO_TEST_OUT 0

// Toggling GPIO 0

int main(int argc, char *argv[])
{
    gpio_t gpio;
    gpio_params_t gpio_params;
    gpio_result_t gpio_res;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_res = gpio_init(gpio_params, &gpio);
    gpio_res = gpio_output_set_enabled(&gpio, GPIO_TEST_OUT, true);

    int gpio_val=0;
    while(1){
        gpio_val = !gpio_val;
        gpio_write(&gpio, GPIO_TEST_OUT, gpio_val);
        for(int i=0; i<1024; i++) {
            asm volatile("nop");
        }
    }


    return EXIT_SUCCESS;
}
