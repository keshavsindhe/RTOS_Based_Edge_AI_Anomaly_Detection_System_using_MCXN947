/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "../include/mpu6050.h"
#include "fsl_debug_console.h"
#include "fsl_lpi2c.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "fsl_common.h"
#include "board.h"

/* LPI2C instance - Arduino Header A4/A5 configuration */
#define MPU6050_LPI2C_MASTER      LPI2C1
#define LPI2C_MASTER_CLOCK_FREQUENCY CLOCK_GetLPFlexCommClkFreq(1U)
#define MPU6050_I2C_ADDR_7BIT       (MPU6050_I2C_ADDR)

static bool i2c_initialized = false;

/**
 * @brief Convert LPI2C status code to human-readable string
 */
static const char* MPU6050_StatusCodeToString(status_t status)
{
    switch (status)
    {
        case kStatus_Success:
            return "kStatus_Success (0)";
        case kStatus_LPI2C_Busy:
            return "kStatus_LPI2C_Busy (900)";
        case kStatus_LPI2C_Idle:
            return "kStatus_LPI2C_Idle (901)";
        case kStatus_LPI2C_Nak:
            return "kStatus_LPI2C_Nak (902) - Slave NAK";
        case kStatus_LPI2C_FifoError:
            return "kStatus_LPI2C_FifoError (903)";
        case kStatus_LPI2C_BitError:
            return "kStatus_LPI2C_BitError (904)";
        case kStatus_LPI2C_ArbitrationLost:
            return "kStatus_LPI2C_ArbitrationLost (905)";
        case kStatus_LPI2C_PinLowTimeout:
            return "kStatus_LPI2C_PinLowTimeout (906) - Bus stuck";
        case kStatus_LPI2C_NoTransferInProgress:
            return "kStatus_LPI2C_NoTransferInProgress (907)";
        case kStatus_LPI2C_DmaRequestFail:
            return "kStatus_LPI2C_DmaRequestFail (908)";
        case kStatus_LPI2C_Timeout:
            return "kStatus_LPI2C_Timeout (909) - TIMEOUT";
        default:
            return "UNKNOWN STATUS CODE";
    }
}

/**
 * @brief Parse and print MSR register flags
 */
static void MPU6050_ParseMSR(uint32_t msr, const char *prefix)
{
    PRINTF("%sMSR Register Analysis:\r\n", prefix);
    PRINTF("%s  TDF (Transmit Data Flag):  %s\r\n", prefix, (msr & (1U << 0)) ? "SET" : "CLEAR");
    PRINTF("%s  RDF (Receive Data Flag):   %s\r\n", prefix, (msr & (1U << 1)) ? "SET" : "CLEAR");
    PRINTF("%s  EPF (End Packet Flag):     %s\r\n", prefix, (msr & (1U << 8)) ? "SET" : "CLEAR");
    PRINTF("%s  SDF (Stop Detection Flag): %s\r\n", prefix, (msr & (1U << 9)) ? "SET" : "CLEAR");
    PRINTF("%s  NDF (NACK Detection Flag): %s [ERROR]\r\n", prefix, (msr & (1U << 10)) ? "SET" : "CLEAR");
    PRINTF("%s  ALF (Arbitration Lost):    %s [ERROR]\r\n", prefix, (msr & (1U << 11)) ? "SET" : "CLEAR");
    PRINTF("%s  FEF (FIFO Error):          %s [ERROR]\r\n", prefix, (msr & (1U << 12)) ? "SET" : "CLEAR");
    PRINTF("%s  PLTF (Pin Low Timeout):    %s [ERROR]\r\n", prefix, (msr & (1U << 13)) ? "SET" : "CLEAR");
    PRINTF("%s  DMF (Data Match Flag):     %s\r\n", prefix, (msr & (1U << 14)) ? "SET" : "CLEAR");
    PRINTF("%s  MBF (Master Busy Flag):    %s\r\n", prefix, (msr & (1U << 24)) ? "BUSY" : "IDLE");
    PRINTF("%s  BBF (Bus Busy Flag):       %s\r\n", prefix, (msr & (1U << 25)) ? "BUSY" : "IDLE");
}

/**
 * @brief Detailed register dump with analysis
 */
static void MPU6050_DumpLPI2CRegs(const char *reason)
{
    uint32_t mcr = MPU6050_LPI2C_MASTER->MCR;
    uint32_t msr = MPU6050_LPI2C_MASTER->MSR;
    uint32_t mfsr = MPU6050_LPI2C_MASTER->MFSR;

    PRINTF("\r\n[LPI2C4][%s] REGISTER DUMP:\r\n", reason);
    PRINTF("[LPI2C4][%s] MCR  = 0x%08X\r\n", reason, (unsigned int)mcr);
    PRINTF("[LPI2C4][%s] MSR  = 0x%08X\r\n", reason, (unsigned int)msr);
    PRINTF("[LPI2C4][%s] MFSR = 0x%08X\r\n", reason, (unsigned int)mfsr);
    
    MPU6050_ParseMSR(msr, "[LPI2C4][Diagnostic] ");
    
    PRINTF("[LPI2C4][%s] MFSR Analysis:\r\n", reason);
    PRINTF("[LPI2C4][%s]  TXCOUNT (TX FIFO): %u\r\n", reason, (unsigned int)((mfsr >> 0) & 0x7F));
    PRINTF("[LPI2C4][%s]  RXCOUNT (RX FIFO): %u\r\n", reason, (unsigned int)((mfsr >> 16) & 0x7F));
    PRINTF("\r\n");
}

static bool MPU6050_TransferRegister(uint8_t reg, lpi2c_direction_t direction, uint8_t *data, size_t dataSize, const char *ctx)
{
    lpi2c_master_transfer_t transfer;
    status_t status;
    uint32_t msr_before = MPU6050_LPI2C_MASTER->MSR;
    uint32_t mfsr_before = MPU6050_LPI2C_MASTER->MFSR;
    const char *dir_str = (direction == kLPI2C_Read) ? "Read" : "Write";

    transfer.flags = kLPI2C_TransferDefaultFlag;
    transfer.slaveAddress = MPU6050_I2C_ADDR_7BIT;
    transfer.direction = direction;
    transfer.subaddress = reg;
    transfer.subaddressSize = 1U;
    transfer.data = data;
    transfer.dataSize = dataSize;

    PRINTF("[LPI2C4][%s] Transfer Configuration:\r\n", ctx);
    PRINTF("[LPI2C4][%s]  Slave Address: 0x%02X (7-bit)\r\n", ctx, (unsigned int)transfer.slaveAddress);
    PRINTF("[LPI2C4][%s]  Direction: %s\r\n", ctx, dir_str);
    PRINTF("[LPI2C4][%s]  Register (subaddress): 0x%02X\r\n", ctx, (unsigned int)reg);
    PRINTF("[LPI2C4][%s]  Data Size: %u bytes\r\n", ctx, (unsigned int)dataSize);

    PRINTF("[LPI2C4][%s] Pre-Transfer State:\r\n", ctx);
    PRINTF("[LPI2C4][%s]  MSR = 0x%08X\r\n", ctx, (unsigned int)msr_before);
    PRINTF("[LPI2C4][%s]  MFSR = 0x%08X\r\n", ctx, (unsigned int)mfsr_before);

    if (msr_before & (1U << 25))  /* BBF - Bus Busy Flag */
    {
        PRINTF("[LPI2C4][%s] [WARNING] Bus is busy (BBF=1)\r\n", ctx);
    }
    if (msr_before & (1U << 10))  /* NDF - NACK Detection Flag */
    {
        PRINTF("[LPI2C4][%s] [WARNING] NACK detected (NDF=1)\r\n", ctx);
    }
    if (msr_before & (1U << 13))  /* PLTF - Pin Low Timeout Flag */
    {
        PRINTF("[LPI2C4][%s] [ERROR] Pin low timeout detected (PLTF=1)\r\n", ctx);
    }

    PRINTF("[LPI2C4][%s] Executing LPI2C_MasterTransferBlocking()...\r\n", ctx);
    status = LPI2C_MasterTransferBlocking(MPU6050_LPI2C_MASTER, &transfer);

    PRINTF("[LPI2C4][%s] Transfer Complete:\r\n", ctx);
    PRINTF("[LPI2C4][%s]  Status Code: %d\r\n", ctx, (int)status);
    PRINTF("[LPI2C4][%s]  Status Name: %s\r\n", ctx, MPU6050_StatusCodeToString(status));

    if (status != kStatus_Success)
    {
        uint32_t msr_after = MPU6050_LPI2C_MASTER->MSR;
        uint32_t mfsr_after = MPU6050_LPI2C_MASTER->MFSR;
        
        PRINTF("[LPI2C4][%s] Post-Transfer State (on error):\r\n", ctx);
        PRINTF("[LPI2C4][%s]  MSR = 0x%08X\r\n", ctx, (unsigned int)msr_after);
        PRINTF("[LPI2C4][%s]  MFSR = 0x%08X\r\n", ctx, (unsigned int)mfsr_after);
        
        MPU6050_DumpLPI2CRegs("TRANSFER_FAILED");
        return false;
    }

    PRINTF("[LPI2C4][%s] Transfer succeeded!\r\n\r\n", ctx);
    return true;
}

/**
 * @brief Initialize LPI2C peripheral for MPU6050
 */
static void MPU6050_I2C_Init(void)
{
    lpi2c_master_config_t masterConfig;
    uint32_t sourceClock;
    uint32_t status_reg;

    if (i2c_initialized)
    {
        PRINTF("[DEBUG] LPI2C1 already initialized\r\n");
        return;
    }

    PRINTF("\r\n================================\r\n");
    PRINTF("LPI2C1 INITIALIZATION (Arduino A4/A5)\r\n");
    PRINTF("================================\r\n\n");

    PRINTF("[DEBUG] Target: MPU6050 at I2C address 0x%02X (7-bit)\r\n\n", (unsigned int)MPU6050_I2C_ADDR_7BIT);
    
    /* Enable PORT0 clock */
    PRINTF("[DEBUG] Enabling PORT0 clock...\r\n");
    CLOCK_EnableClock(kCLOCK_Port0);

    /* Configure Arduino A4/A5: P0_15 (SDA), P0_23 (SCL) */
    const port_pin_config_t i2cPin = {
        .pullSelect = kPORT_PullUp,
        .pullValueSelect = kPORT_HighPullResistor,
        .slewRate = kPORT_FastSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterDisable,
        .openDrainEnable = kPORT_OpenDrainEnable,
        .driveStrength = kPORT_HighDriveStrength,
        .mux = kPORT_MuxAlt2,  /* FC1 I2C function */
        .inputBuffer = kPORT_InputBufferEnable,
        .invertInput = kPORT_InputNormal,
        .lockRegister = kPORT_UnlockRegister,
    };
    PRINTF("[DEBUG] Configuring P0_15 (ARD_A4) as SDA...\r\n");
    PORT_SetPinConfig(PORT0, 15U, &i2cPin);
    PRINTF("[DEBUG] Configuring P0_23 (ARD_A5) as SCL...\r\n");
    PORT_SetPinConfig(PORT0, 23U, &i2cPin);
    
    /* Enable LPI2C1 clock */
    PRINTF("[DEBUG] Enabling LPI2C1 clock...\r\n");
    CLOCK_EnableClock(kCLOCK_LPFlexComm1);
    
    /* Attach clock to LPI2C1 */
    PRINTF("[DEBUG] Attaching FRO12M to LPI2C1...\r\n");
    CLOCK_AttachClk(kFRO12M_to_FLEXCOMM1);
    
    /* Verify clock */
    sourceClock = LPI2C_MASTER_CLOCK_FREQUENCY;
    PRINTF("[DEBUG] Clock Configuration:\r\n");
    PRINTF("[DEBUG]  LPI2C1 Source Clock: %u Hz\r\n", (unsigned int)sourceClock);
    
    if (sourceClock == 0)
    {
        PRINTF("[ERROR] LPI2C1 clock is 0! Board initialization may have failed.\r\n");
        PRINTF("[ERROR] Check:\r\n");
        PRINTF("[ERROR]  1. CLOCK_InitFROMU() called in board initialization\r\n");
        PRINTF("[ERROR]  2. LPI2C1 clock gate enabled\r\n");
        PRINTF("[ERROR]  3. MCUXpresso board support package correct\r\n");
        return;
    }
    
    /* Get default configuration */
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    
    PRINTF("[DEBUG] LPI2C1 Configuration:\r\n");
    masterConfig.baudRate_Hz = 100000U;  /* 100kHz */
    masterConfig.busIdleTimeout_ns = 10000000U;   /* 10ms */
    masterConfig.pinLowTimeout_ns = 10000000U;    /* 10ms */
    
    PRINTF("[DEBUG]  Baud Rate: 100 kHz\r\n");
    PRINTF("[DEBUG]  Bus Idle Timeout: 10 ms\r\n");
    PRINTF("[DEBUG]  Pin Low Timeout: 10 ms\r\n");
    PRINTF("[DEBUG]  Pins: P0_23 (SCL/A5), P0_15 (SDA/A4)\r\n");
    
    PRINTF("\n[DEBUG] Calling LPI2C_MasterInit()...\r\n");
    LPI2C_MasterInit(MPU6050_LPI2C_MASTER, &masterConfig, sourceClock);
    PRINTF("[DEBUG] LPI2C1 initialized successfully\r\n");
    
    /* Verify bus state after init */
    status_reg = MPU6050_LPI2C_MASTER->MSR;
    PRINTF("[DEBUG] Post-init Bus Status (MSR):\r\n");
    PRINTF("[DEBUG]  MSR = 0x%08X\r\n", (unsigned int)status_reg);
    PRINTF("[DEBUG]  Master Busy (MBF):  %s\r\n", (status_reg & (1U << 24)) ? "YES" : "NO");
    PRINTF("[DEBUG]  Bus Busy (BBF):     %s\r\n", (status_reg & (1U << 25)) ? "YES" : "NO");
    
    if (status_reg & (1U << 25))  /* BBF - Bus Busy Flag */
    {
        PRINTF("[WARNING] I2C bus is BUSY after initialization!\r\n");
        PRINTF("[WARNING] Attempting bus recovery...\r\n");
        LPI2C_MasterReset(MPU6050_LPI2C_MASTER);
        SDK_DelayAtLeastUs(10000, SystemCoreClock);  /* 10ms delay */
        
        status_reg = MPU6050_LPI2C_MASTER->MSR;
        PRINTF("[DEBUG] After recovery, MSR = 0x%08X\r\n", (unsigned int)status_reg);
        
        if (status_reg & (1U << 25))
        {
            PRINTF("[ERROR] Bus still busy after recovery! Check:\r\n");
            PRINTF("[ERROR]  1. I2C slave device connected?\r\n");
            PRINTF("[ERROR]  2. SDA/SCL pins pulled low?\r\n");
            PRINTF("[ERROR]  3. Pull-up resistors installed (should be 4.7k Ohm)?\r\n");
            PRINTF("[ERROR]  4. Correct pin multiplexing?\r\n");
        }
    }
    
    PRINTF("[DEBUG] Ready to communicate with MPU6050\r\n");
    PRINTF("================================\r\n\n");
    
    i2c_initialized = true;
}

/**
 * @brief Write a single register to MPU6050
 */
bool MPU6050_WriteRegister(uint8_t reg, uint8_t data)
{
    uint8_t tx_data = data;

    if (!i2c_initialized)
    {
        MPU6050_I2C_Init();
    }

    return MPU6050_TransferRegister(reg, kLPI2C_Write, &tx_data, 1U, "WriteRegister");
}

/**
 * @brief Read a single register from MPU6050
 */
bool MPU6050_ReadRegister(uint8_t reg, uint8_t *data)
{
    if (!i2c_initialized)
    {
        MPU6050_I2C_Init();
    }

    PRINTF("[DEBUG] Reading register 0x%02X...\r\n", reg);
    PRINTF("[DEBUG] Using 7-bit slave address 0x%02X on LPI2C1\r\n", (unsigned int)MPU6050_I2C_ADDR_7BIT);

    if (!MPU6050_TransferRegister(reg, kLPI2C_Read, data, 1U, "ReadRegister"))
    {
        return false;
    }

    PRINTF("[DEBUG]   Read complete: 0x%02X\r\n", *data);
    return true;
}

/**
 * @brief Initialize MPU6050 sensor
 */
bool MPU6050_Init(void)
{
    uint8_t who_am_i = 0U;
    uint8_t pwr_mgmt_1 = 0U;
    uint8_t accel_raw[6] = {0U};

    PRINTF("================================\r\n");
    PRINTF("MPU6050 INITIALIZATION\r\n");
    PRINTF("================================\r\n\n");

    PRINTF("[PHASE 1] Initialize I2C bus\r\n");
    MPU6050_I2C_Init();

    PRINTF("[PHASE 2] Read WHO_AM_I Register (0x75)\r\n");
    PRINTF("[DEBUG] This verifies the MPU6050 is present and responding\r\n\n");
    
    if (!MPU6050_ReadRegister(MPU6050_WHO_AM_I, &who_am_i))
    {
        PRINTF("\r\n[ERROR] Failed to read WHO_AM_I\r\n");
        PRINTF("[ERROR] Check:\r\n");
        PRINTF("[ERROR]  1. Is MPU6050 powered? (VCC = 3.3V)\r\n");
        PRINTF("[ERROR]  2. Is MPU6050 soldered on board?\r\n");
        PRINTF("[ERROR]  3. Are SDA/SCL connected to correct pins?\r\n");
        PRINTF("[ERROR]  4. Are pull-up resistors installed?\r\n");
        PRINTF("[ERROR]  5. Status code 909 = Timeout (slave not responding)\r\n");
        return false;
    }

    PRINTF("\n[SUCCESS] WHO_AM_I Read Successful!\r\n");
    PRINTF("          WHO_AM_I = 0x%02X\r\n\n", who_am_i);
    
    if (who_am_i != MPU6050_WHO_AM_I_VAL)
    {
        PRINTF("[ERROR] WHO_AM_I mismatch!\r\n");
        PRINTF("[ERROR] Expected: 0x%02X (MPU6050 with AD0=GND)\r\n", MPU6050_WHO_AM_I_VAL);
        PRINTF("[ERROR] Got:      0x%02X\r\n", who_am_i);
        PRINTF("[ERROR] Check:\r\n");
        PRINTF("[ERROR]  1. Is correct sensor installed?\r\n");
        PRINTF("[ERROR]  2. Is AD0 pin connected to GND (not VCC)?\r\n");
        return false;
    }

    PRINTF("[PHASE 3] Wake Up MPU6050 (PWR_MGMT_1)\r\n");
    PRINTF("[DEBUG] Writing 0x00 to PWR_MGMT_1 (0x6B) to wake from sleep\r\n");
    PRINTF("[DEBUG] Bit 6 (SLEEP) = 0 to enable power\r\n\n");
    
    if (!MPU6050_WriteRegister(MPU6050_PWR_MGMT_1, 0x00U))
    {
        PRINTF("[ERROR] Failed to write PWR_MGMT_1\r\n");
        return false;
    }

    PRINTF("[DEBUG] Waiting 10ms for device to wake...\r\n");
    SDK_DelayAtLeastUs(10000U, SystemCoreClock);

    PRINTF("[PHASE 4] Verify PWR_MGMT_1 Write\r\n");
    PRINTF("[DEBUG] Reading back PWR_MGMT_1 to verify write succeeded\r\n\n");
    
    if (!MPU6050_ReadRegister(MPU6050_PWR_MGMT_1, &pwr_mgmt_1))
    {
        PRINTF("[ERROR] Failed to read back PWR_MGMT_1\r\n");
        return false;
    }

    PRINTF("[SUCCESS] PWR_MGMT_1 Read Successful!\r\n");
    PRINTF("          PWR_MGMT_1 = 0x%02X\r\n\n", pwr_mgmt_1);
    
    if (pwr_mgmt_1 != 0x00U)
    {
        PRINTF("[WARNING] PWR_MGMT_1 not as expected (0x00)\r\n");
        PRINTF("[WARNING] Got: 0x%02X - Device may not be fully awake\r\n", pwr_mgmt_1);
    }

    PRINTF("[PHASE 5] Read Accelerometer Data (6 bytes)\r\n");
    PRINTF("[DEBUG] Reading ACCEL_XOUT_H through ACCEL_ZOUT_L (0x3B-0x40)\r\n\n");
    
    if (!MPU6050_TransferRegister(MPU6050_ACCEL_XOUT_H, kLPI2C_Read, accel_raw, sizeof(accel_raw), "Init.ReadAccel"))
    {
        PRINTF("[ERROR] Failed to read accelerometer registers\r\n");
        return false;
    }

    PRINTF("[SUCCESS] Accelerometer Read Successful!\r\n");
    PRINTF("          Raw Data: X=0x%02X%02X Y=0x%02X%02X Z=0x%02X%02X\r\n",
           accel_raw[0], accel_raw[1], accel_raw[2], accel_raw[3], accel_raw[4], accel_raw[5]);
    
    /* Parse as signed 16-bit values */
    int16_t accel_x = (int16_t)((accel_raw[0] << 8) | accel_raw[1]);
    int16_t accel_y = (int16_t)((accel_raw[2] << 8) | accel_raw[3]);
    int16_t accel_z = (int16_t)((accel_raw[4] << 8) | accel_raw[5]);
    
    float g_x = (float)accel_x / 16384.0f;
    float g_y = (float)accel_y / 16384.0f;
    float g_z = (float)accel_z / 16384.0f;
    
    PRINTF("          Acceleration: X=%.4f g  Y=%.4f g  Z=%.4f g\r\n", g_x, g_y, g_z);
    PRINTF("          Expected: X≈0g  Y≈0g  Z≈1g (if stationary)\r\n\n");

    PRINTF("================================\r\n");
    PRINTF("MPU6050 INITIALIZATION COMPLETE\r\n");
    PRINTF("================================\r\n\n");

    return true;
}

/**
 * @brief Print diagnostic registers
 */
void MPU6050_PrintDiagnostics(void)
{
    uint8_t pwr_mgmt_1, pwr_mgmt_2, accel_config;

    PRINTF("================================\r\n");
    PRINTF("MPU6050 Diagnostics\r\n");
    PRINTF("================================\r\n\n");

    if (MPU6050_ReadRegister(MPU6050_PWR_MGMT_1, &pwr_mgmt_1))
    {
        PRINTF("PWR_MGMT_1 (0x6B) = 0x%02X\r\n", pwr_mgmt_1);
    }
    else
    {
        PRINTF("ERROR: Failed to read PWR_MGMT_1\r\n");
    }

    if (MPU6050_ReadRegister(MPU6050_PWR_MGMT_2, &pwr_mgmt_2))
    {
        PRINTF("PWR_MGMT_2 (0x6C) = 0x%02X\r\n", pwr_mgmt_2);
    }
    else
    {
        PRINTF("ERROR: Failed to read PWR_MGMT_2\r\n");
    }

    if (MPU6050_ReadRegister(MPU6050_ACCEL_CONFIG, &accel_config))
    {
        PRINTF("ACCEL_CONFIG (0x1C) = 0x%02X\r\n", accel_config);
    }
    else
    {
        PRINTF("ERROR: Failed to read ACCEL_CONFIG\r\n");
    }

    PRINTF("\n");
}

/**
 * @brief Read raw accelerometer data
 */
bool MPU6050_ReadAccel(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t data[6] = {0U};

    if (!i2c_initialized)
    {
        MPU6050_I2C_Init();
    }

    if (!MPU6050_TransferRegister(MPU6050_ACCEL_XOUT_H, kLPI2C_Read, data, sizeof(data), "ReadAccel"))
    {
        return false;
    }

    /* Combine high and low bytes */
    *x = (int16_t)((data[0] << 8) | data[1]);
    *y = (int16_t)((data[2] << 8) | data[3]);
    *z = (int16_t)((data[4] << 8) | data[5]);

    return true;
}

/**
 * @brief Convert raw value to g units
 */
void MPU6050_ConvertToG(int16_t raw, float *g)
{
    *g = (float)raw / MPU6050_ACCEL_SCALE;
}

/**
 * @brief Collect sensor samples into buffer
 */
void CollectSensorSamples(float *buffer, uint32_t size)
{
    int16_t accel_x, accel_y, accel_z;
    float g_value;

    PRINTF(">>> Collecting %u sensor samples (X-axis)...\r\n", (unsigned int)size);

    for (uint32_t i = 0; i < size; i++)
    {
        if (MPU6050_ReadAccel(&accel_x, &accel_y, &accel_z))
        {
            MPU6050_ConvertToG(accel_x, &g_value);
            buffer[i] = g_value;
        }
        else
        {
            PRINTF("ERROR: Failed to read accelerometer at sample %u\r\n", (unsigned int)i);
            buffer[i] = 0.0f;
        }

        /* Small delay between samples (~1ms for ~1kHz sampling rate) */
        SDK_DelayAtLeastUs(1000, SystemCoreClock);
    }

    PRINTF("Collected %u samples\r\n\n", (unsigned int)size);
}

/**
 * @brief Test I2C communication at a specific address
 * @param addr_7bit 7-bit I2C address to test
 * @param addr_name Name of address (for display)
 * @return true if device responds with WHO_AM_I = 0x68, false otherwise
 */
bool MPU6050_TestAddress(uint8_t addr_7bit, const char *addr_name)
{
    lpi2c_master_transfer_t transfer;
    uint8_t who_am_i = 0U;
    status_t status;

    PRINTF("\n[ADDRESS TEST] Testing %s (0x%02X)...\r\n", addr_name, (unsigned int)addr_7bit);

    /* Initialize transfer for WHO_AM_I read */
    transfer.flags = kLPI2C_TransferDefaultFlag;
    transfer.slaveAddress = addr_7bit;  /* 7-bit address */
    transfer.direction = kLPI2C_Read;
    transfer.subaddress = 0x75;  /* WHO_AM_I register */
    transfer.subaddressSize = 1U;
    transfer.data = &who_am_i;
    transfer.dataSize = 1U;

    PRINTF("[ADDRESS TEST]  Address: 0x%02X (7-bit)\r\n", (unsigned int)addr_7bit);
    PRINTF("[ADDRESS TEST]  Reading WHO_AM_I (0x75)...\r\n");

    /* Attempt transfer */
    status = LPI2C_MasterTransferBlocking(MPU6050_LPI2C_MASTER, &transfer);

    PRINTF("[ADDRESS TEST]  Status: %s\r\n", MPU6050_StatusCodeToString(status));

    if (status == kStatus_Success)
    {
        PRINTF("[ADDRESS TEST]  WHO_AM_I = 0x%02X\r\n", who_am_i);

        if (who_am_i == 0x68)
        {
            PRINTF("[SUCCESS] ✓ MPU6050 found at %s (0x%02X)\r\n\n", addr_name, (unsigned int)addr_7bit);
            return true;
        }
        else
        {
            PRINTF("[WARNING] Device responds but WHO_AM_I = 0x%02X (expected 0x68)\r\n\n", who_am_i);
            return false;
        }
    }
    else
    {
        PRINTF("[FAILED] ✗ No response at %s (0x%02X)\r\n\n", addr_name, (unsigned int)addr_7bit);
        return false;
    }
}

/**
 * @brief Scan both valid MPU6050 addresses (0x68 and 0x69)
 * @return true if device found, false if not found at either address
 */
bool MPU6050_ScanAddresses(void)
{
    PRINTF("\n================================\r\n");
    PRINTF("MPU6050 I2C ADDRESS SCAN\r\n");
    PRINTF("================================\r\n");
    PRINTF("This will test both possible addresses.\r\n");
    PRINTF("MPU6050 address depends on AD0 pin:\r\n");
    PRINTF("  AD0 = GND → 0x68\r\n");
    PRINTF("  AD0 = VCC → 0x69\r\n\n");

    /* Ensure I2C is initialized */
    if (!i2c_initialized)
    {
        PRINTF("[DEBUG] Initializing LPI2C1 for address scan...\r\n");
        MPU6050_I2C_Init();
    }

    /* Test address 0x68 (AD0 = GND) */
    bool found_68 = MPU6050_TestAddress(0x68, "Address AD0=GND");

    /* Test address 0x69 (AD0 = VCC) */
    bool found_69 = MPU6050_TestAddress(0x69, "Address AD0=VCC");

    PRINTF("================================\r\n");
    PRINTF("ADDRESS SCAN RESULTS\r\n");
    PRINTF("================================\r\n\n");

    if (found_68)
    {
        PRINTF("✓ MPU6050 found at 0x68 (AD0 = GND)\r\n");
        return true;
    }
    else if (found_69)
    {
        PRINTF("✓ MPU6050 found at 0x69 (AD0 = VCC)\r\n");
        PRINTF("[NOTE] Please update MPU6050_I2C_ADDR in mpu6050.h to 0x69\r\n");
        return true;
    }
    else
    {
        PRINTF("✗ MPU6050 NOT FOUND at either address (0x68 or 0x69)\r\n");
        PRINTF("[ERROR] Check:\r\n");
        PRINTF("[ERROR]  1. Is MPU6050 powered? (VCC = 3.3V)\r\n");
        PRINTF("[ERROR]  2. Is MPU6050 soldered on board?\r\n");
        PRINTF("[ERROR]  3. Are SDA/SCL connected correctly?\r\n");
        PRINTF("[ERROR]  4. Are pull-up resistors installed?\r\n");
        return false;
    }
}
