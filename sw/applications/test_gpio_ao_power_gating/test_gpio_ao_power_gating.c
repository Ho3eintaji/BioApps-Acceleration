// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <stdlib.h>
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "gpio.h"
#include "fast_intr_ctrl.h"
#include "fast_intr_ctrl_regs.h"
#include "power_manager.h"

#define GPIO_TB_OUT 0
#define GPIO_TB_IN  1

gpio_t gpio;

static power_manager_t power_manager;

int main(int argc, char *argv[])
{
    // Setup gpio
    gpio_params_t gpio_params;
    gpio_result_t gpio_res;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_res = gpio_init(gpio_params, &gpio);
    if (gpio_res != kGpioOk) {
        printf("Fail.\n;");
        return -1;
    }

    // Setup fast interrupt controller
    fast_intr_ctrl_t fast_intr_ctrl;
    fast_intr_ctrl.base_addr = mmio_region_from_addr((uintptr_t)FAST_INTR_CTRL_START_ADDRESS);

    // Setup power_manager
    mmio_region_t power_manager_reg = mmio_region_from_addr(POWER_MANAGER_START_ADDRESS);
    power_manager.base_addr = power_manager_reg;
    power_manager_counters_t power_manager_cpu_counters;

    // Init cpu_subsystem's counters
    if (power_gate_counters_init(&power_manager_cpu_counters, 30, 30, 30, 30, 30, 30, 0, 0) != kPowerManagerOk_e)
    {
        printf("Error: power manager fail. Check the reset and powergate counters value\n");
        return EXIT_FAILURE;
    }

    gpio_res = gpio_output_set_enabled(&gpio, GPIO_TB_OUT, true);
    if (gpio_res != kGpioOk) {
        printf("Fail.\n;");
        return -1;
    }

    gpio_res = gpio_irq_set_trigger(&gpio, 1 << GPIO_TB_IN, kGpioIrqTriggerLevelHigh);
    if (gpio_res != kGpioOk) {
        printf("Fail.\n;");
        return -1;
    }

    gpio_res = gpio_irq_set_enabled(&gpio, GPIO_TB_IN, true);
    if (gpio_res != kGpioOk) {
        printf("Fail.\n;");
        return -1;
    }

    printf("Write 1 to GPIO 0 and wait for interrupt...");
    gpio_write(&gpio, GPIO_TB_OUT, true);
    CSR_CLEAR_BITS(CSR_REG_MSTATUS, 0x8);
    if (power_gate_core(&power_manager, kGpio_1_pm_e, &power_manager_cpu_counters) != kPowerManagerOk_e)
    {
        printf("Error: power manager fail.\n");
        return EXIT_FAILURE;
    }
    CSR_SET_BITS(CSR_REG_MSTATUS, 0x8);
    clear_fast_interrupt(&fast_intr_ctrl, kGpio_1_fic_e);

    printf("Success.\n");
    return EXIT_SUCCESS;
}
