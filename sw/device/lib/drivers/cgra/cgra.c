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


#include <stddef.h>
#include <stdint.h>

#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "cgra.h"
#include "cgra_bitstream.h"
#include "cgra_regs.h" // generated

void cgra_cmem_init(void)
{
  int32_t *cgra_cmem_ptr = (int32_t*) (CGRA_START_ADDRESS);

  for (int i=0; i<CGRA_IMEM_SIZE; i++) {
    *cgra_cmem_ptr++ = cgra_imem_bistream[i];
  }

  for (int i=0; i<CGRA_KMEM_SIZE; i++) {
    *cgra_cmem_ptr++ = cgra_kem_bitstream[i];
  }
}

void cgra_set_read_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t read_ptr, uint8_t column) {
  if (slot_id == 0) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_PTR_IN_C0_REG_OFFSET+0x8*column), read_ptr);
  } else if (slot_id == 1) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT1_PTR_IN_C0_REG_OFFSET+0x8*column), read_ptr);
  }
}

void cgra_set_write_ptr(const cgra_t *cgra, uint8_t slot_id, uint32_t write_ptr, uint8_t column) {
  if (slot_id == 0) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_PTR_OUT_C0_REG_OFFSET+0x8*column), write_ptr);
  } else if (slot_id == 1) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT1_PTR_OUT_C0_REG_OFFSET+0x8*column), write_ptr);
  }
}

void cgra_set_kernel(const cgra_t *cgra, uint8_t slot_id, uint32_t kernel_id) {
  // Only slot 0 and 1 work for now
  if (slot_id == 0) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT0_KER_ID_REG_OFFSET), kernel_id);
  } else if (slot_id == 1) {
    mmio_region_write32(cgra->base_addr, (ptrdiff_t)(CGRA_SLOT1_KER_ID_REG_OFFSET), kernel_id);
  }
}
