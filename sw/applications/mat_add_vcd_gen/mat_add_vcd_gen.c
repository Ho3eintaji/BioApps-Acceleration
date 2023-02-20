// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include <stdio.h>
#include <stdlib.h>
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "core_v_mini_mcu.h"
#include "data.h"
#include "gpio.h"

#define VCD_TRIGGER_GPIO 0

static gpio_t gpio;
int32_t m_c[16*16];

void __attribute__ ((noinline)) matrixAdd(int32_t *A, int32_t *B, int32_t *C, int N, int M);
uint32_t check_results(int32_t *C, int N, int M);

void dump_on(void);
void dump_off(void);

int main(int argc, char *argv[])
{
    int N = WIDTH;
    int M = HEIGHT;

    uint32_t errors = 0;
    unsigned int cycles;

    dump_on();
    CSR_WRITE(CSR_REG_MCYCLE, 0);

    matrixAdd(m_a, m_b, m_c, N, M);

    CSR_READ(CSR_REG_MCYCLE, &cycles);
    dump_off();

    errors = check_results(m_c, N, M);

    printf("Program finished with %d errors and %d cycles.\n", errors, cycles);

    return EXIT_SUCCESS;
}

void dump_on(void)
{
    gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, &gpio);
    gpio_output_set_enabled(&gpio, VCD_TRIGGER_GPIO, true);

    gpio_write(&gpio, VCD_TRIGGER_GPIO, true);
}

void dump_off(void)
{
    gpio_write(&gpio, VCD_TRIGGER_GPIO, false);
}

void __attribute__ ((noinline)) matrixAdd(int32_t *A, int32_t *B, int32_t *C, int N, int M)
{
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            C[i*N+j] = A[i*WIDTH+j] + B[i*WIDTH+j];
        }
    }
}

uint32_t check_results(int32_t *C, int N, int M)
{
    int i, j;
    uint32_t err = 0;

    for(i = 0; i < N; i++) {
        for(j = 0; j < M; j++) {
            if(C[i*N+j] != m_exp[i*WIDTH+j]) {
                err++;
                printf("Error at index %d, %d, expected %d, got %d.\n", i, j, m_exp[i*WIDTH+j], C[i*N+j]);
            }
        }
    }

    return err;
}
