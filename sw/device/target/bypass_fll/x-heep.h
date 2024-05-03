// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef X_HEEP
#define X_HEEP

#pragma message ( "the x-heep.h for HEEPOCRATES when bypassing the FLL is used" )
#pragma message ( "!!!!! DO NOT FORGET to run simulation with bypass_fll=1 !!!!!" )

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define REFERENCE_CLOCK_Hz 100*1000*1000
#define REFERENCE_CLOCK_Hz_LOG2 26
#define UART_BAUDRATE 256000
#define TARGET_BYPASS_FLL


#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // X_HEEP
