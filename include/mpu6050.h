/*
 * Copyright 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>
#include <stdbool.h>

/* MPU6050 I2C Address */
#define MPU6050_I2C_ADDR        0x68

/* MPU6050 Register Map */
#define MPU6050_WHO_AM_I        0x75
#define MPU6050_PWR_MGMT_1      0x6B
#define MPU6050_PWR_MGMT_2      0x6C
#define MPU6050_ACCEL_CONFIG    0x1C
#define MPU6050_ACCEL_XOUT_H    0x3B
#define MPU6050_ACCEL_XOUT_L    0x3C
#define MPU6050_ACCEL_YOUT_H    0x3D
#define MPU6050_ACCEL_YOUT_L    0x3E
#define MPU6050_ACCEL_ZOUT_H    0x3F
#define MPU6050_ACCEL_ZOUT_L    0x40

/* Expected WHO_AM_I value */
#define MPU6050_WHO_AM_I_VAL    0x68

/* Accelerometer sensitivity (±2g range) */
#define MPU6050_ACCEL_SCALE     16384.0f

/* Function prototypes */
bool MPU6050_Init(void);
bool MPU6050_ReadRegister(uint8_t reg, uint8_t *data);
bool MPU6050_WriteRegister(uint8_t reg, uint8_t data);
bool MPU6050_ReadAccel(int16_t *x, int16_t *y, int16_t *z);
void MPU6050_ConvertToG(int16_t raw, float *g);
void MPU6050_PrintDiagnostics(void);
void CollectSensorSamples(float *buffer, uint32_t size);
bool MPU6050_TestAddress(uint8_t addr_7bit, const char *addr_name);
bool MPU6050_ScanAddresses(void);

#endif /* MPU6050_H */
