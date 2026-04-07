#ifndef __ADXL_
#define __ADXL_

#include "main.h"
#include "sensors.h"

#define ADXL372_READ  0x01
#define ADXL372_WRITE 0x00

#define ADXL372_POWER_CTL     0x3E
#define ADXL372_MEASURE       0x3D
#define ADXL372_TIMING        0x3C

static uint16_t triaxial_x_g;
static uint16_t triaxial_y_g;
static uint16_t triaxial_z_g;

extern SPI_HandleTypeDef hspi3;

// typedef struct {
//     int16_t x, y, z;
// } ADXL372_Data_t;

uint8_t ADXL372_ReadReg(uint8_t reg, uint16_t chip_select) {
    uint8_t addr = (reg << 1) | ADXL372_READ;
    uint8_t val = 0;

    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, &addr, 1, 10);
    HAL_SPI_Receive(&hspi3, &val, 1, 10);
    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);

    return val;
}

void ADXL372_WriteReg(uint8_t reg, uint8_t val, uint16_t chip_select) {
    uint8_t tx_buf[2];
    tx_buf[0] = (reg << 1) | ADXL372_WRITE;
    tx_buf[1] = val;

    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, tx_buf, 2, 10);
    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
}

void ADXL372_Init(uint16_t cs) {
    // 1. Soft Reset (Optional but recommended)
    ADXL372_WriteReg(0x41, 0x52, cs); // Reset code 'R'
    HAL_Delay(100); 

    // 2. Configure Timing (Register 0x3C)
    // Set ODR to 3200 Hz (0x80)
    ADXL372_WriteReg(ADXL372_TIMING, 0x80, cs);

    // 3. Configure Measurement Settings (Register 0x3D)
    // Set bandwidth to 1600Hz (half ODR) and low noise mode
    ADXL372_WriteReg(ADXL372_MEASURE, 0x00, cs); 

    // 4. Power Control (Register 0x3E)
    // Set to Full Measurement Mode (0x03)
    // This is what actually "starts" the sensor.
    ADXL372_WriteReg(ADXL372_POWER_CTL, 0x03, cs);
}

// void Sample_ADXL372(ADXL372_Data_t *data, uint16_t chip_select) {
//     uint8_t raw[6];
//     uint8_t addr = (0x08 << 1) | ADXL372_READ; // Start at XDATA_H

//     HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
//     HAL_SPI_Transmit(&hspi3, &addr, 1, 10);
//     HAL_SPI_Receive(&hspi3, raw, 6, 10);
//     HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);

//     // Data is 12-bit, left-justified in 16-bit registers (typically)
//     // Check datasheet for justification. For ADXL372:
//     data->x = (int16_t)((raw[0] << 4) | (raw[1] >> 4));
//     data->y = (int16_t)((raw[2] << 4) | (raw[3] >> 4));
//     data->z = (int16_t)((raw[4] << 4) | (raw[5] >> 4));
    
//     // Convert to mg: (value * 100)
// }

void Sample_ADXL372(uint16_t chip_select) {
    uint8_t raw[6];
    uint8_t addr = (0x08 << 1) | ADXL372_READ; // Start at XDATA_H

    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, &addr, 1, 10);
    HAL_SPI_Receive(&hspi3, raw, 6, 10);
    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);

    // Data is 12-bit, left-justified in 16-bit registers (typically)
    // Check datasheet for justification. For ADXL372:
    triaxial_x_g = (int16_t)((raw[0] << 4) | (raw[1] >> 4));
    triaxial_y_g = (int16_t)((raw[2] << 4) | (raw[3] >> 4));
    triaxial_z_g = (int16_t)((raw[4] << 4) | (raw[5] >> 4));
    
    // Convert to mg: (value * 100)
}

#endif
