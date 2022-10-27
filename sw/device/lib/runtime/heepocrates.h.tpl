// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

#ifndef HEEPOCRATES_H_
#define HEEPOCRATES_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "core_v_mini_mcu.h"

#define EXT_XBAR_NMASTER ${xbar_nmasters}
#define EXT_XBAR_NSLAVE  ${xbar_nsalves}

#define CGRA_START_ADDRESS (EXT_SLAVE_START_ADDRESS + 0x${cgra_start_address})
#define CGRA_SIZE 0x${cgra_size_address}
#define CGRA_END_ADDRESS (CGRA_START_ADDRESS + CGRA_SIZE)

#define CGRA_PERIPH_START_ADDRESS (EXT_PERIPHERAL_START_ADDRESS + 0x${cgra_periph_start_address})
#define CGRA_PERIPH_SIZE 0x${cgra_periph_size_address}
#define CGRA_PERIPH_END_ADDRESS (CGRA_PERIPH_START_ADDRESS + CGRA_PERIPH_SIZE)

#define FLL_START_ADDRESS (EXT_PERIPHERAL_START_ADDRESS + 0x${fll_start_address})
#define FLL_SIZE 0x${fll_size_address}
#define FLL_END_ADDRESS (FLL_START_ADDRESS + FLL_SIZE)

#define COUBERTIN_START_ADDRESS (EXT_PERIPHERAL_START_ADDRESS + 0x${coubertin_periph_start_address})
#define COUBERTIN_SIZE 0x${coubertin_periph_size_address}
#define COUBERTIN_END_ADDRESS (COUBERTIN_START_ADDRESS + FLL_SIZE)

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // HEEPOCRATES_H_
