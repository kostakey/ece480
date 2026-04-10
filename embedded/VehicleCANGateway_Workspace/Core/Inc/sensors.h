#ifndef __SENSORS_
#define __SENSORS_

#include "adxl.h"

#define ADC_BITS 12
#define ADC_MAX_VALUE 4095
#define ADC_VREF 2400 // vref in mV
#define IA_GAIN 100

// define sensor signals here, also create processing functions here -- shared between the gatewaya and the sensor board
// keep in mind, each signal is 16 bits

// need general ADC to raw voltage

// strain gauge things -- from ADC
static uint16_t strain_gauge_adc;
//static uint16_t strain_gauge_plus_v;
//static uint16_t strain_gauge_minus_adc;
//static uint16_t strain_gauge_minus_v;
//static uint16_t strain_gauge_diff_adc;
static uint16_t strain_gauge_diff_v;
static uint16_t strain_gauge_hz; // strain gauge frequency - see if oscillations can be picked up here

// uniaxial things -- from ADC
static uint16_t uniaxial_adc;
static uint16_t uniaxial_v; // raw voltage
static uint16_t uniaxial_hz; // frequency

// triaxial things -- 16 bit sigs from SPI
static uint16_t triaxial_x_g;
static uint16_t triaxial_y_g;
static uint16_t triaxial_z_g;

//extern int16_t triaxial_x_raw;
//extern int16_t triaxial_y_raw;
//extern int16_t triaxial_z_raw;

//triaxial_x_g = ADXL372_GetG(triaxial_x_raw);
//triaxial_y_g = ADXL372_GetG(triaxial_y_raw);
//triaxial_z_g = ADXL372_GetG(triaxial_z_raw);

uint16_t ADC_to_Voltage(uint16_t adc){
    return (uint16_t)((uint32_t)adc * ADC_VREF) / ADC_MAX_VALUE;
}

float Raw_to_G(int16_t raw_val) {
    // 1. Handle Sign Extension for 12-bit data
    // The ADXL372 uses 12 bits. If bit 11 (0x800) is set, it's negative.
    if (raw_val & 0x0800) {
        // Apply sign extension: Fill bits 12-15 with 1s
        raw_val |= 0xF000;
    } else {
        // Ensure bits 12-15 are 0s for positive numbers
        raw_val &= 0x0FFF;
    }

    // 2. Convert to Gs
    // Sensitivity is 100 mg/LSB.
    // 100 mg = 0.1 G.
    return (float)raw_val * 0.1f;
}

//uint16_t Uniaxial_Voltage_To_Freq(uint16_t v){
//
//    return;
//}
//
//uint16_t Strain_Gauge_Diff_To_Freq(uint16_t v){
//
//    return;
//}

#endif
