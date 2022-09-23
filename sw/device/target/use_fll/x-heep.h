// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef X_HEEP
#define X_HEEP

#pragma message ( "the x-heep.h for HEEPOCRATES when using the FLL is used" )
#pragma message ( "the Value used for the FLL REFERENCE_CLOCK_Hz is taken from Simulation waveform, you should set the FLL in the CRT0" )
//
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


#define REFERENCE_CLOCK_Hz 43014452
#define UART_BAUDRATE 256000
#define TARGET_USE_FLL


#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // X_HEEP
