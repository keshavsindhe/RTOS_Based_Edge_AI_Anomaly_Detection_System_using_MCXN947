/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * MPU6050 Simple Read Test
 * 
 * Wiring:
 *   MPU6050 VCC -> J5 3V3
 *   MPU6050 GND -> J5 GND
 *   MPU6050 SDA -> J5 SDA (P1_0)
 *   MPU6050 SCL -> J5 SCL (P1_1)
 *   MPU6050 AD0 -> GND
 *
 * Just reads MPU6050 accelerometer data continuously.
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "board.h"
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations */
void BOARD_InitHardware(void);

#define MPU6050_ADDR           0x68U
#define MPU6050_PWR_MGMT_1     0x6BU
#define MPU6050_ACCEL_XOUT_H   0x3BU

static void InitI2C(void)
{
    lpi2c_master_config_t config;
    uint32_t sourceClock;
    
    /* Enable clocks */
    CLOCK_EnableClock(kCLOCK_Port1);
    CLOCK_EnableClock(kCLOCK_LPFlexComm3);
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM3);
    
    /* Configure pins P1_0 (SDA) and P1_1 (SCL) */
    const port_pin_config_t i2cPin = {
        .pullSelect = kPORT_PullUp,
        .pullValueSelect = kPORT_HighPullResistor,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainEnable,
        .driveStrength = kPORT_HighDriveStrength,
        .mux = kPORT_MuxAlt2,
        .inputBuffer = kPORT_InputBufferEnable,
        .invertInput = kPORT_InputNormal,
        .lockRegister = kPORT_UnlockRegister,
    };
    PORT_SetPinConfig(PORT1, 0U, &i2cPin);  /* SDA */
    PORT_SetPinConfig(PORT1, 1U, &i2cPin);  /* SCL */
    
    /* Initialize LPI2C3 */
    LPI2C_MasterGetDefaultConfig(&config);
    config.baudRate_Hz = 100000U;
    sourceClock = CLOCK_GetLPFlexCommClkFreq(3U);
    LPI2C_MasterInit(LPI2C3, &config, sourceClock);
    
    PRINTF("I2C initialized\r\n");
}

static bool MPU6050_Write(uint8_t reg, uint8_t value)
{
    lpi2c_master_transfer_t transfer = {0};
    transfer.flags = kLPI2C_TransferDefaultFlag;
    transfer.slaveAddress = MPU6050_ADDR;
    transfer.direction = kLPI2C_Write;
    transfer.subaddress = reg;
    transfer.subaddressSize = 1U;
    transfer.data = &value;
    transfer.dataSize = 1U;
    return (LPI2C_MasterTransferBlocking(LPI2C3, &transfer) == kStatus_Success);
}

static bool MPU6050_Read(uint8_t reg, uint8_t *data, size_t len)
{
    lpi2c_master_transfer_t transfer = {0};
    transfer.flags = kLPI2C_TransferDefaultFlag;
    transfer.slaveAddress = MPU6050_ADDR;
    transfer.direction = kLPI2C_Read;
    transfer.subaddress = reg;
    transfer.subaddressSize = 1U;
    transfer.data = data;
    transfer.dataSize = len;
    return (LPI2C_MasterTransferBlocking(LPI2C3, &transfer) == kStatus_Success);
}

/*******************************************************************************
 * Main Function
 ******************************************************************************/

int main(void)
{
    uint8_t accelData[6];
    int16_t x, y, z;
    
    BOARD_InitHardware();
    
    PRINTF("\r\nMPU6050 Read Test\r\n");
    PRINTF("LPI2C3: P1_0=SDA, P1_1=SCL\r\n\r\n");
    
    InitI2C();
    
    /* Wake up MPU6050 */
    if (MPU6050_Write(MPU6050_PWR_MGMT_1, 0x00))
    {
        PRINTF("MPU6050 wake OK\r\n");
    }
    else
    {
        PRINTF("MPU6050 wake FAILED\r\n");
    }
    
    SDK_DelayAtLeastUs(100000, CLOCK_GetCoreSysClkFreq());  /* 100ms */
    
    PRINTF("\r\nReading accelerometer:\r\n\r\n");
    
    /* Continuous read */
    while (1)
    {
        if (MPU6050_Read(MPU6050_ACCEL_XOUT_H, accelData, 6))
        {
            x = (int16_t)((accelData[0] << 8) | accelData[1]);
            y = (int16_t)((accelData[2] << 8) | accelData[3]);
            z = (int16_t)((accelData[4] << 8) | accelData[5]);
            
            PRINTF("X=%6d  Y=%6d  Z=%6d\r\n", x, y, z);
        }
        else
        {
            PRINTF("ERROR\r\n");
        }
        
        SDK_DelayAtLeastUs(500000, CLOCK_GetCoreSysClkFreq());  /* 500ms */
    }
    
    return 0;
}
