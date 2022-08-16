// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

#ifndef HEEPOCRATES_H_
#define HEEPOCRATES_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define EXT_XBAR_NMASTER ${xbar_nmasters}
#define EXT_XBAR_NSLAVE  ${xbar_nsalves}

#define BLADE_START_ADDRESS 0x${blade_start_address}
#define BLADE_SIZE 0x${blade_size_address}
#define BLADE_END_ADDRESS (BLADE_START_ADDRESS + BLADE_SIZE)

#define CGRA_START_ADDRESS 0x${cgra_start_address}
#define CGRA_SIZE 0x${cgra_size_address}
#define CGRA_END_ADDRESS (CGRA_START_ADDRESS + CGRA_SIZE)

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // HEEPOCRATES_H_
