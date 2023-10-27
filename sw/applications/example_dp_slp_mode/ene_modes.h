/*
 * ene_modes.h
 * Author: Hossein Taji
 * Date: Oct, 2023
 * Version: 1.0   
 * Description: This file implements several energy modes supported by Heepocrates board.
 */

#ifndef ENE_MODES_H
#define ENE_MODES_H

#include <stdio.h>
#include <stdlib.h>
#include "heepocrates.h"
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
#include "gpio.h"
#include "power_manager.h"
#include "csr.h"
#include "hart.h"
#include "handler.h"


void init_ene_modes(uint64_t sys_freq, boolean );
void en_sleep_mode();
void en_deep_sleep_mode();

#endif // ENE_MODES_H