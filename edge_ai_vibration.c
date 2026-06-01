/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "arm_math.h"
#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "edge_ai_vibration/peripherals.h"

void EdgeAIVibration_LegacyInit(void)
{
    BOARD_InitHardware();
    BOARD_InitPeripherals();

    PRINTF("Edge AI Vibration board support initialized\r\n");
}
