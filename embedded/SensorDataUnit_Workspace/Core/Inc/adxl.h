//#ifndef __ADXL_
//#define __ADXL_
//
//#include "main.h"
//#include "sensors.h"
//
//#define ADXL372_READ  0x01
//#define ADXL372_WRITE 0x00
////
////#define ADXL372_POWER_CTL     0x3E
////#define ADXL372_MEASURE       0x3D
////#define ADXL372_TIMING        0x3C
//
//#define ADXL372_REVID          0x03
//#define ADXL372_XDATA_H        0x08
//#define ADXL372_TIMING         0x3D  // Was 0x3C
//#define ADXL372_MEASURE        0x3E  // Was 0x3D
//#define ADXL372_POWER_CTL      0x3F  // Was 0x3E
//#define ADXL372_SRESET         0x41
//
//static uint16_t triaxial_x_g;
//static uint16_t triaxial_y_g;
//static uint16_t triaxial_z_g;
//
//extern SPI_HandleTypeDef hspi3;
//
//// typedef struct {
////     int16_t x, y, z;
//// } ADXL372_Data_t;
//
////uint8_t ADXL372_ReadReg(uint8_t reg, uint16_t chip_select) {
////    uint8_t addr = (reg << 1) | ADXL372_READ;
////    uint8_t val = 0;
////
////    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
////    HAL_SPI_Transmit(&hspi3, &addr, 1, 10);
////    HAL_SPI_Receive(&hspi3, &val, 1, 10);
////    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
////
////    return val;
////}
//
//uint8_t ADXL372_ReadReg(uint8_t reg, uint16_t chip_select) {
//    uint8_t addr = (reg << 1) | 0x01; // ADXL372_READ is 1
//    uint8_t val = 0;
//
//    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
//
//    // Send the address
//    if (HAL_SPI_Transmit(&hspi3, &addr, 1, 10) == HAL_OK) {
//        // Immediately receive the value while CS is still LOW
//        HAL_SPI_Receive(&hspi3, &val, 1, 10);
//    }
//
//    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
//
//    return val;
//}
//
////void ADXL372_WriteReg(uint8_t reg, uint8_t val, uint16_t chip_select) {
////    uint8_t tx_buf[2];
////    tx_buf[0] = (reg << 1) | ADXL372_WRITE;
////    tx_buf[1] = val;
////
////    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
////    HAL_SPI_Transmit(&hspi3, tx_buf, 2, 10);
////    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
////}
//
//void ADXL372_WriteReg(uint8_t reg, uint8_t val, uint16_t chip_select) {
//    uint8_t tx_buf[2];
//    tx_buf[0] = (reg << 1) | 0x00; // ADXL372_WRITE is 0
//    tx_buf[1] = val;
//
//    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
//    // Transmitting 2 bytes in one burst is the correct way to write
//    HAL_SPI_Transmit(&hspi3, tx_buf, 2, 10);
//    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
//}
//
////void ADXL372_Init(uint16_t cs) {
////    // 1. Soft Reset (Optional but recommended)
////    ADXL372_WriteReg(0x41, 0x52, cs); // Reset code 'R'
////    HAL_Delay(100);
////
////    // 2. Configure Timing (Register 0x3C)
////    // Set ODR to 3200 Hz (0x80)
////    ADXL372_WriteReg(ADXL372_TIMING, 0x80, cs);
////
////    // 3. Configure Measurement Settings (Register 0x3D)
////    // Set bandwidth to 1600Hz (half ODR) and low noise mode
////    ADXL372_WriteReg(ADXL372_MEASURE, 0x00, cs);
////
////    // 4. Power Control (Register 0x3E)
////    // Set to Full Measurement Mode (0x03)
////    // This is what actually "starts" the sensor.
////    ADXL372_WriteReg(ADXL372_POWER_CTL, 0x03, cs);
////}
//
//void ADXL372_Init(uint16_t cs) {
//    uint8_t devId = ADXL372_ReadReg(0x00, cs);
//    if (devId != 0xAD) {
//        // ERROR: Bus not working or wrong wiring
//        return;
//    }
//
//    ADXL372_WriteReg(ADXL372_SRESET, 0x52, cs);
//    HAL_Delay(100);
//
//    // ODR 6400Hz, High pass disabled
//    ADXL372_WriteReg(ADXL372_TIMING, 0x80, cs);
//    // Bandwidth 3200Hz
//    ADXL372_WriteReg(ADXL372_MEASURE, 0x04, cs);
//    // Full measurement mode
//    ADXL372_WriteReg(ADXL372_POWER_CTL, 0x03, cs);
//}
//
//// void Sample_ADXL372(ADXL372_Data_t *data, uint16_t chip_select) {
////     uint8_t raw[6];
////     uint8_t addr = (0x08 << 1) | ADXL372_READ; // Start at XDATA_H
//
////     HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
////     HAL_SPI_Transmit(&hspi3, &addr, 1, 10);
////     HAL_SPI_Receive(&hspi3, raw, 6, 10);
////     HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
//
////     // Data is 12-bit, left-justified in 16-bit registers (typically)
////     // Check datasheet for justification. For ADXL372:
////     data->x = (int16_t)((raw[0] << 4) | (raw[1] >> 4));
////     data->y = (int16_t)((raw[2] << 4) | (raw[3] >> 4));
////     data->z = (int16_t)((raw[4] << 4) | (raw[5] >> 4));
//
////     // Convert to mg: (value * 100)
//// }
//
//void Sample_ADXL372(uint16_t chip_select) {
//    uint8_t raw[6];
//    // Address for XDATA_H is 0x08.
//    uint8_t addr = (0x08 << 1) | ADXL372_READ;
//
//    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
//    if(HAL_SPI_Transmit(&hspi3, &addr, 1, 10) == HAL_OK) {
//        HAL_SPI_Receive(&hspi3, raw, 6, 10);
//    }
//    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
//
//    // Reconstruct 12-bit signed data
//    // The LSB register (raw[1], [3], [5]) has data in bits [7:4]
//    int16_t x = (int16_t)((raw[0] << 4) | (raw[1] >> 4));
//    int16_t y = (int16_t)((raw[2] << 4) | (raw[3] >> 4));
//    int16_t z = (int16_t)((raw[4] << 4) | (raw[5] >> 4));
//
//    // Handle sign extension for negative values (12-bit to 16-bit)
//    if (x > 2047) x -= 4096;
//    if (y > 2047) y -= 4096;
//    if (z > 2047) z -= 4096;
//
//    triaxial_x_g = x;
//    triaxial_y_g = y;
//    triaxial_z_g = z;
//}
//
//#endif

#ifndef __ADXL_H_
#define __ADXL_H_

#include "main.h"

/* --- ADXL372 Register Map --- */
#define ADXL372_DEVID_AD      0x00
#define ADXL372_DEVID_MST     0x01
#define ADXL372_PARTID        0x02
#define ADXL372_REVID         0x03
#define ADXL372_STATUS_1      0x04
#define ADXL372_XDATA_H       0x08
#define ADXL372_XDATA_L       0x09
#define ADXL372_YDATA_H       0x0A
#define ADXL372_YDATA_L       0x0B
#define ADXL372_ZDATA_H       0x0C
#define ADXL372_ZDATA_L       0x0D
#define ADXL372_TIMING        0x3D
#define ADXL372_MEASURE       0x3E
#define ADXL372_POWER_CTL     0x3F
#define ADXL372_SRESET        0x41

/* --- SPI Protocol Defines --- */
#define ADXL372_READ          0x01
#define ADXL372_WRITE         0x00

/* --- External Hardware Handle --- */
extern SPI_HandleTypeDef hspi3;

/* --- Data Storage --- */
// These are visible to main.c because they are static in a header (if included in multiple files,
// they will have separate copies. For a better practice, use 'extern' here and define in a .c file).
static int16_t triaxial_x_raw;
static int16_t triaxial_y_raw;
static int16_t triaxial_z_raw;

/**
 * @brief Write to an ADXL372 register
 */
static inline void ADXL372_WriteReg(uint8_t reg, uint8_t val, uint16_t cs_pin) {
    uint8_t tx_buf[2];
    tx_buf[0] = (reg << 1) | ADXL372_WRITE;
    tx_buf[1] = val;

    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, tx_buf, 2, 10);
    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_SET);
}

/**
 * @brief Read from an ADXL372 register
 */
static inline uint8_t ADXL372_ReadReg(uint8_t reg, uint16_t cs_pin) {
    uint8_t addr = (reg << 1) | ADXL372_READ;
    uint8_t val = 0;

    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, &addr, 1, 10) == HAL_OK) {
        HAL_SPI_Receive(&hspi3, &val, 1, 10);
    }
    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_SET);

    return val;
}

/**
 * @brief Initialize the ADXL372
 * @return 1 if success (Device ID found), 0 if failed
 */
static inline uint8_t ADXL372_Init(uint16_t cs_pin) {
    // 1. Verify Connectivity
    uint8_t devId = ADXL372_ReadReg(ADXL372_DEVID_AD, cs_pin);
    if (devId != 0xAD) {
        return 0; // Hardware fail
    }

    // 2. Software Reset
    ADXL372_WriteReg(ADXL372_SRESET, 0x52, cs_pin);
    HAL_Delay(100);

    // 3. Configure Timing (ODR: 6400 Hz)
    ADXL372_WriteReg(ADXL372_TIMING, 0x80, cs_pin);

    // 4. Configure Measurement (Bandwidth: 3200 Hz, Low Noise)
    ADXL372_WriteReg(ADXL372_MEASURE, 0x04, cs_pin);
    
    // 5. Power Control: Full Measurement Mode
    ADXL372_WriteReg(ADXL372_POWER_CTL, 0x03, cs_pin);

    return 1;
}

/**
 * @brief Burst read X, Y, and Z axes
 */
static inline void Sample_ADXL372(uint16_t cs_pin) {
    uint8_t raw[6];
    uint8_t addr = (ADXL372_XDATA_H << 1) | ADXL372_READ;

    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_RESET);
    if (HAL_SPI_Transmit(&hspi3, &addr, 1, 10) == HAL_OK) {
        // Multi-byte read: Chip auto-increments address from 0x08 through 0x0D
        HAL_SPI_Receive(&hspi3, raw, 6, 10);
    }
    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_SET);

    /* * ADXL372 Data Format: 12-bit, Left Justified
     * Register H: [D11 D10 D9 D8 D7 D6 D5 D4]
     * Register L: [D3  D2  D1 D0  X  X  X  X ]
     */
    int16_t x = (int16_t)((raw[0] << 4) | (raw[1] >> 4));
    int16_t y = (int16_t)((raw[2] << 4) | (raw[3] >> 4));
    int16_t z = (int16_t)((raw[4] << 4) | (raw[5] >> 4));

    // Sign Extension: 12-bit to 16-bit
    if (x > 2047) x -= 4096;
    if (y > 2047) y -= 4096;
    if (z > 2047) z -= 4096;

    triaxial_x_raw = x;
    triaxial_y_raw = y;
    triaxial_z_raw = z;
}

/**
 * @brief Optional: Convert raw value to Gs
 * Max range is ±200G. Sensitivity is 100 mg/LSB.
 */
static inline float ADXL372_GetG(int16_t raw_value) {
    return (float)raw_value * 0.1f;
}

#endif /* __ADXL_H_ */
