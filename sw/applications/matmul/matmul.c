// Copyright 2023 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

#include "heepocrates.h"
#include "core_v_mini_mcu.h"

#include "matmul_data.h"
#include "gpio.h"


#define GPIO_TEST_OUT 0

#ifdef DEBUG_APPLICATION
    #pragma message ( "DEBUG_APPLICATION is defined, if the matmul fails it prints the expected value" )
#endif

#ifdef TOGGLE_GPIO
    #pragma message ( "TOGGLE_GPIO is defined, if MEASURE_POWER_APPLICATION is defined, then every 1000 matmul it toggles GPIO0" )
#endif

#ifdef MEASURE_POWER_APPLICATION
    #pragma message ( "MEASURE_POWER_APPLICATION is defined, the application never ends" )
#endif

int __attribute__ ((noinline)) matmul(int N, int* matrixM, int* matrixA, int * C, int* calculated_output)
{

    for(int i=0; i < N; i++){
        for(int j=0; j < N; j++){
            int c = 0;
            for(int k=0; k < N; k++){
                c += matrixM[i*N + k] * matrixA[k*N + j];
            }
            C[i*N + j] = c;
            #ifdef DEBUG_APPLICATION
            if(c != calculated_output[i*N + j]) {
                printf("Expected %d, got %d\n",c,calculated_output[i] );
                return -1;
            }
            #endif //DEBUG_APPLICATION
        }
    }

    return 0;
}

int output_C[OUTPUT_EXPECTED_C_SIZE];

int main(int argc, char const *argv[])
{


    int gpio_val=0;
    int gpio_cnt = 0;

    #ifdef TOGGLE_GPIO
    gpio_t gpio;
    gpio_params_t gpio_params;
    gpio_result_t gpio_res;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_res = gpio_init(gpio_params, &gpio);
    gpio_res = gpio_output_set_enabled(&gpio, GPIO_TEST_OUT, true);
    #endif // TOGGLE_GPIO

    #ifdef MEASURE_POWER_APPLICATION
    while(1) {
    #endif //MEASURE_POWER_APPLICATION
        gpio_cnt++;

        matmul(MATMUL_RC_SIZE, input_M, input_A, output_C, output_expected_C);

        #ifdef TOGGLE_GPIO
            if(gpio_cnt > 1000){
                gpio_val = !gpio_val;
                gpio_write(&gpio, GPIO_TEST_OUT, gpio_val);
                gpio_cnt = 0;
            }
        #endif // TOGGLE_GPIO


    #ifdef MEASURE_POWER_APPLICATION
    }
    #endif //MEASURE_POWER_APPLICATION


    return 0;
}
