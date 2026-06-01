/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "arm_math.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"

void BOARD_InitHardware(void)
{
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
}
