// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

#ifndef HEEPOCRATES_H_
#define HEEPOCRATES_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define EXT_XBAR_NMASTER 4
#define EXT_XBAR_NSLAVE  2

#define BLADE_START_ADDRESS 0x30000000
#define BLADE_SIZE 0x100000
#define BLADE_END_ADDRESS (BLADE_START_ADDRESS + BLADE_SIZE)

#define CGRA_START_ADDRESS 0x30100000
#define CGRA_SIZE 0x100000
#define CGRA_END_ADDRESS (CGRA_START_ADDRESS + CGRA_SIZE)

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // HEEPOCRATES_H_
