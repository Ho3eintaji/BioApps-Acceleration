////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                 Name Surname - email (affiliation if not ESL)              //
//                                                                            //
// File Name:      cgra.h                                                     //
//                                                                            //
// Project Name:   CGRA_ESL                                                   //
//                                                                            //
// Language:       C                                                          //
//                                                                            //
// Description:    CGRA parameters and control functions.                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _CGRA_H_
#define _CGRA_H_

#include <stdint.h>

#include "mmio.h"

#define CGRA_INTR 58

#define CGRA_KMEM_SIZE 16
#define CGRA_IMEM_SIZE 512
#define CGRA_N_SLOTS   2

// Kernel 0 => NULL
#define DBL_MIN_KER_ID      1
#define DBL_MAX_KER_ID      2
#define MAX_PEAK_KER_ID     3
#define MIN_MAX_CIRC_KER_ID 4
#define BIT_COUNT_2C_KER_ID 5
#define BIT_COUNT_1C_KER_ID 6
// One or the other
#define BIT_COUNT_3C_KER_ID 7
#define BIT_COUNT_4C_KER_ID 7

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Write the CGRA bistream to its memory
 */
void cgra_cmem_init(void);

/**
 * Initialization parameters for CGRA peripheral control registers.
 *
 */
typedef struct cgra {
  /**
   * The base address for the soc_ctrl hardware registers.
   */
  mmio_region_t base_addr;
} cgra_t;

/**
 * Write to read_ptr register of the CGRA control registers
 * @param cgra Pointer to cgra_t represting the target MEMCOPY PERIPHERAL.
 * @param slot_id Slot ID to which write the read_ptr address
 * @param read_ptr Any valid memory address.
 * @param column Column number to which write the read_ptr address
 */
void cgra_set_read_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t read_ptr, uint8_t column);

/**
 * Write to write_ptr register of the CGRA control registers
 * @param cgra Pointer to cgra_t represting the target MEMCOPY PERIPHERAL.
 * @param slot_id Slot ID to which write the write_ptr address
 * @param write_ptr Any valid memory address.
 * @param column Column number to which write the write_ptr address
 */
void cgra_set_write_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t write_ptr, uint8_t column);

/**
 * Write to kernel_id register of the CGRA control registers
 * @param cgra Pointer to cgra_t represting the target MEMCOPY PERIPHERAL.
 * @param slot_id Slot ID to which request the kernel_id
 * @param kernel_id Kernel ID to execute
 */
void cgra_set_kernel(const cgra_t *cgra, uint8_t slot_id, uint32_t kernel_id);

#ifdef __cplusplus
}
#endif

#endif // _CGRA_H_