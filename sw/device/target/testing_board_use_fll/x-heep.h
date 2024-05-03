// Copyright EPFL contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef X_HEEP
#define X_HEEP

#pragma message ( "the x-heep.h for the HEEPOCRATES TESTING BOARD when the FLL is used" )
#pragma message ( "!!!!! DO NOT FORGET to run set the BYPASS HEEP switch to L !!!!!" )

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define REFERENCE_CLOCK_Hz 32768
#define REFERENCE_CLOCK_Hz_LOG2 15
#define UART_BAUDRATE 9600
#define TARGET_USE_FLL
#define USE_TESTING_BOARD


#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // X_HEEP
