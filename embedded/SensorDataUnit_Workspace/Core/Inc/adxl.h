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
    uint8_t tx_buf[2] = { (uint8_t)((reg << 1) | ADXL372_READ), 0x00 };
    uint8_t rx_buf[2] = { 0, 0 };

    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_RESET);
    // Use the 2-byte size to ensure the SPI pulses exactly 16 times
    HAL_SPI_TransmitReceive(&hspi3, tx_buf, rx_buf, 2, 10);
    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_SET);

    return rx_buf[1]; // The actual data is in the second slot
}

/**
 * @brief Initialize the ADXL372
 * @return 1 if success (Device ID found), 0 if failed
 */
void ADXL372_Init(uint16_t cs_pin) {
    // 1. Verify Connectivity
    ADXL372_ReadReg(ADXL372_DEVID_AD, cs_pin);
    ADXL372_ReadReg(ADXL372_DEVID_AD, cs_pin); // 2nd one to flush out FIFO buffers

    // 2. Software Reset
    ADXL372_WriteReg(ADXL372_SRESET, 0x52, cs_pin);
    HAL_Delay(100);

    // 3. Configure Timing (ODR: 6400 Hz)
    ADXL372_WriteReg(ADXL372_TIMING, 0x80, cs_pin);

    // 4. Configure Measurement (Bandwidth: 3200 Hz, Low Noise)
    ADXL372_WriteReg(ADXL372_MEASURE, 0x04, cs_pin);
    
    // 5. Power Control: Full Measurement Mode
    ADXL372_WriteReg(ADXL372_POWER_CTL, 0x03, cs_pin);

}

/**
 * @brief Burst read X, Y, and Z axes
 */
static inline void Sample_ADXL372(uint16_t cs_pin) {
    uint8_t addr = (ADXL372_XDATA_H << 1) | ADXL372_READ;
    uint8_t tx_buf[7] = {addr, 0, 0, 0, 0, 0, 0}; // Address + 6 dummy bytes
    uint8_t rx_buf[7];

    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_RESET);

    // Use TransmitReceive for the whole 7-byte block (Address + 6 Data Bytes)
    // This is much faster and more reliable on WL33
    if (HAL_SPI_TransmitReceive(&hspi3, tx_buf, rx_buf, 7, 10) == HAL_OK) {
        // rx_buf[0] is garbage (received while sending address)
        // Data starts from rx_buf[1]
        int16_t x = (int16_t)((rx_buf[1] << 4) | (rx_buf[2] >> 4));
        int16_t y = (int16_t)((rx_buf[3] << 4) | (rx_buf[4] >> 4));
        int16_t z = (int16_t)((rx_buf[5] << 4) | (rx_buf[6] >> 4));

        if (x > 2047) x -= 4096;
        if (y > 2047) y -= 4096;
        if (z > 2047) z -= 4096;

        triaxial_x_raw = x;
        triaxial_y_raw = y;
        triaxial_z_raw = z;
    }
    HAL_GPIO_WritePin(GPIOA, cs_pin, GPIO_PIN_SET);
}

/**
 * @brief Optional: Convert raw value to Gs
 * Max range is ±200G. Sensitivity is 100 mg/LSB.
 */
static inline float ADXL372_GetG(int16_t raw_value) {
    return (float)raw_value * 0.1f;
}

#endif /* __ADXL_H_ */
