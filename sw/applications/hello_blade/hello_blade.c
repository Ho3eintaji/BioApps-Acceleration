/*
 * Copyright 2020 ETH Zurich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Robert Balas <balasr@iis.ee.ethz.ch>
 */

#include <stdio.h>
#include <stdlib.h>
#include "heepocrates.h"
#include "core_v_mini_mcu.h"

int main(int argc, char *argv[])
{
    //should be replaced with Blade library
    int blade_wvalue = 0xDA41DE;
    int blade_rvalue = 0;
    volatile unsigned int* blade_ptr =  (unsigned int*)BLADE_START_ADDRESS;

    /* write something to stdout */
    printf("writing %x to blade!\n", blade_wvalue);

    blade_ptr[0] = blade_wvalue;
    blade_rvalue = blade_ptr[0];

    printf("reading %x from blade!\n", blade_rvalue);

    return blade_rvalue == blade_wvalue ? EXIT_SUCCESS : EXIT_FAILURE;
}
