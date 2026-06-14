/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * MPU6050 Matrix Bring-Up
 * Board  : FRDM-MCXN947
 * Goal   : Find any peripheral/pin configuration that returns WHO_AM_I = 0x68
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "board.h"
#include <stdint.h>
#include <stdbool.h>

void BOARD_InitHardware(void);

#define I2C_BAUD_HZ      100000U
#define MPU6050_ADDR      0x68U
#define REG_WHO_AM_I      0x75U

typedef struct
{
    const char *peripheralName;
    LPI2C_Type *base;
    clock_ip_name_t flexcommClock;
    clock_attach_id_t flexcommAttach;
    uint32_t flexcommIndex;
    PORT_Type *port;
    uint32_t sdaPin;
    uint32_t sclPin;
    const char *pinsLabel;
} i2c_test_row_t;

typedef struct
{
    bool initOk;
    uint32_t srcClockHz;
    status_t status;
    uint8_t who;
} i2c_test_result_t;

static const i2c_test_row_t s_testMatrix[] = {
    {"LPI2C2", LPI2C2, kCLOCK_LPFlexComm2, kFRO12M_to_FLEXCOMM2, 2U, PORT1, 8U, 9U, "SDA=P1_8 SCL=P1_9"},
    {"LPI2C3", LPI2C3, kCLOCK_LPFlexComm3, kFRO12M_to_FLEXCOMM3, 3U, PORT1, 0U, 1U, "SDA=P1_0 SCL=P1_1"},
};

static status_t I2C_ReadRegStatus(LPI2C_Type *base, uint8_t addr7, uint8_t reg, uint8_t *value)
{
    lpi2c_master_transfer_t transfer = {0};
    transfer.flags = kLPI2C_TransferDefaultFlag;
    transfer.slaveAddress = addr7;
    transfer.direction = kLPI2C_Read;
    transfer.subaddress = reg;
    transfer.subaddressSize = 1U;
    transfer.data = value;
    transfer.dataSize = 1U;
    return LPI2C_MasterTransferBlocking(base, &transfer);
}

static void ConfigurePins(PORT_Type *port, uint32_t sdaPin, uint32_t sclPin)
{
    const port_pin_config_t pinConfig = {
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

    PORT_SetPinConfig(port, sdaPin, &pinConfig);
    PORT_SetPinConfig(port, sclPin, &pinConfig);
}

static bool InitI2cRow(const i2c_test_row_t *row, uint32_t *srcClockHz)
{
    lpi2c_master_config_t config;

    CLOCK_EnableClock(kCLOCK_Port1);
    CLOCK_EnableClock(row->flexcommClock);
    CLOCK_AttachClk(row->flexcommAttach);

    ConfigurePins(row->port, row->sdaPin, row->sclPin);

    LPI2C_MasterGetDefaultConfig(&config);
    config.baudRate_Hz = I2C_BAUD_HZ;
    config.busIdleTimeout_ns = 10000000U;
    config.pinLowTimeout_ns = 10000000U;

    *srcClockHz = CLOCK_GetLPFlexCommClkFreq(row->flexcommIndex);
    if (*srcClockHz == 0U)
    {
        return false;
    }

    LPI2C_MasterInit(row->base, &config, *srcClockHz);
    return true;
}

static void DeinitI2cRow(const i2c_test_row_t *row)
{
    LPI2C_MasterDeinit(row->base);
}

static void PrintEnumeration(void)
{
    uint32_t i;

    PRINTF("LPI2C instances on MCXN947: %u\r\n", (unsigned int)FSL_FEATURE_SOC_LPI2C_COUNT);
    for (i = 0U; i < FSL_FEATURE_SOC_LPI2C_COUNT; i++)
    {
        PRINTF("  - LPI2C%u\r\n", (unsigned int)i);
    }

    PRINTF("\r\nKnown FRDM-MCXN947 SDA/SCL pin pairs from SDK pin-mux data:\r\n");
    PRINTF("  - P1_0 / P1_1  -> FC3_P0 / FC3_P1 (LPI2C3 path)\r\n");
    PRINTF("  - P1_8 / P1_9  -> FC4_P0 / FC4_P1 (typically LPI2C4 path)\r\n");
    PRINTF("  - P4_0 / P4_1  -> FC2_P0 / FC2_P1 (LPI2C2 path in SDK examples)\r\n\r\n");
}

int main(void)
{
    uint32_t i;
    bool found = false;
    i2c_test_result_t results[sizeof(s_testMatrix) / sizeof(s_testMatrix[0])] = {0};

    BOARD_InitHardware();

    PRINTF("\r\n====================================================\r\n");
    PRINTF(" MPU6050 Peripheral/Pin Matrix Test - FRDM-MCXN947\r\n");
    PRINTF("====================================================\r\n\r\n");

    PrintEnumeration();

    PRINTF("Requested test matrix:\r\n");
    for (i = 0U; i < (uint32_t)(sizeof(s_testMatrix) / sizeof(s_testMatrix[0])); i++)
    {
        uint8_t who = 0U;
        const i2c_test_row_t *row = &s_testMatrix[i];

        results[i].initOk = InitI2cRow(row, &results[i].srcClockHz);
        if (results[i].initOk)
        {
            results[i].status = I2C_ReadRegStatus(row->base, MPU6050_ADDR, REG_WHO_AM_I, &who);
            results[i].who = who;
            DeinitI2cRow(row);
        }
        else
        {
            results[i].status = kStatus_Fail;
            results[i].who = 0U;
        }

        /* Restore console path in case this row reused debug UART pins. */
        BOARD_InitDebugConsole();
    }

    for (i = 0U; i < (uint32_t)(sizeof(s_testMatrix) / sizeof(s_testMatrix[0])); i++)
    {
        const i2c_test_row_t *row = &s_testMatrix[i];

        PRINTF("----------------------------------------------------\r\n");
        PRINTF("Peripheral: %s\r\n", row->peripheralName);
        PRINTF("Pins      : %s\r\n", row->pinsLabel);
        PRINTF("Clock     : %lu Hz\r\n", (unsigned long)results[i].srcClockHz);

        if (!results[i].initOk)
        {
            PRINTF("Result    : FAIL, source clock is 0 Hz (clock attach/setup issue)\r\n");
            continue;
        }

        if ((results[i].status == kStatus_Success) && (results[i].who == 0x68U))
        {
            PRINTF("Result    : SUCCESS, WHO_AM_I = 0x%02X\r\n", results[i].who);
            found = true;
        }
        else if (results[i].status == kStatus_Success)
        {
            PRINTF("Result    : FAIL, WHO_AM_I = 0x%02X (expected 0x68)\r\n", results[i].who);
        }
        else
        {
            PRINTF("Result    : FAIL, status = %d\r\n", (int)results[i].status);
        }
    }

    PRINTF("----------------------------------------------------\r\n");
    if (found)
    {
        PRINTF("Goal      : Found a configuration with WHO_AM_I = 0x68\r\n");
    }
    else
    {
        PRINTF("Goal      : No matrix row returned WHO_AM_I = 0x68\r\n");
    }

    PRINTF("\r\nLooping forever.\r\n");
    while (1)
    {
        SDK_DelayAtLeastUs(1000000U, SystemCoreClock);
    }
}
