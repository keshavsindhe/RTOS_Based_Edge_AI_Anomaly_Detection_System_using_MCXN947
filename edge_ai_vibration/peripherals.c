/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "arm_math.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_debug_console.h"
#include "peripherals.h"
#include "../dsp/dsp_impl.h"

void BOARD_InitPeripherals(void)
{
    PRINTF("[DEBUG] BOARD_InitPeripherals() starting...\r\n");
    
    /* Enable LPI2C3 clock - FRDM-MCXN947 verified configuration */
    PRINTF("[DEBUG] Enabling LPI2C3 clock...\r\n");
    CLOCK_EnableClock(kCLOCK_LPFlexComm3);
    CLOCK_EnableClock(kCLOCK_Port1);
    
    PRINTF("[DEBUG] Configuring I2C3 pins (P1_0=SDA, P1_1=SCL)...\r\n");
    
    /* Configure I2C3 pins: P1_0 = SDA, P1_1 = SCL */
    const port_pin_config_t i2c_pin_config = {
        .pullSelect = kPORT_PullUp,
        .pullValueSelect = kPORT_HighPullResistor,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainEnable,
        .driveStrength = kPORT_HighDriveStrength,
        .mux = kPORT_MuxAlt2,  /* LPI2C3 function */
        .inputBuffer = kPORT_InputBufferEnable,
        .invertInput = kPORT_InputNormal,
        .lockRegister = kPORT_UnlockRegister
    };
    
    /* Configure SDA pin (P1_0) */
    PORT_SetPinConfig(PORT1, 0U, &i2c_pin_config);
    
    /* Configure SCL pin (P1_1) */
    PORT_SetPinConfig(PORT1, 1U, &i2c_pin_config);

    /* Read back PCR settings to verify actual mux/open-drain configuration. */
    {
        uint32_t pcr_sda = PORT1->PCR[0U];
        uint32_t pcr_scl = PORT1->PCR[1U];
        uint32_t mux_sda = (pcr_sda & PORT_PCR_MUX_MASK) >> PORT_PCR_MUX_SHIFT;
        uint32_t mux_scl = (pcr_scl & PORT_PCR_MUX_MASK) >> PORT_PCR_MUX_SHIFT;
        uint32_t ode_sda = (pcr_sda & PORT_PCR_ODE_MASK) ? 1U : 0U;
        uint32_t ode_scl = (pcr_scl & PORT_PCR_ODE_MASK) ? 1U : 0U;

        PRINTF("[DEBUG] PIN VERIFY: SDA=P1_0 PCR=0x%08X MUX=%u ODE=%u\r\n", (unsigned int)pcr_sda, (unsigned int)mux_sda, (unsigned int)ode_sda);
        PRINTF("[DEBUG] PIN VERIFY: SCL=P1_1 PCR=0x%08X MUX=%u ODE=%u\r\n", (unsigned int)pcr_scl, (unsigned int)mux_scl, (unsigned int)ode_scl);
        PRINTF("[DEBUG] PIN VERIFY EXPECTED: Peripheral=LPI2C3, SDA=P1_0, SCL=P1_1, MUX=ALT2\r\n");
    }
    
    PRINTF("[DEBUG] I2C3 pins configured successfully\r\n");
    PRINTF("[DEBUG] BOARD_InitPeripherals() complete\r\n\n");
    
    /* Initialize ADC for accelerometer readings */
    /* TODO: Configure ADC peripheral for sensor input */
    
    /* Initialize timer for periodic sampling */
    /* TODO: Set up timer interrupt for 8kHz sampling */
}
