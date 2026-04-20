#ifndef __SENSORS_
#define __SENSORS_

#include "adxl.h"
#include <stdlib.h>

#define ADC_BITS 12
#define ADC_MAX_VALUE 4095.0
#define ADC_VREF 2400.0 // vref in mV
//#define ADC_VREF2 1400 // vref in mV
#define IA_GAIN 13

#define GAUGE_FACTOR 2.04
#define EXCITATION 3.3

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
static int16_t strain;
static uint16_t strain_gauge_hz; // strain gauge frequency - see if oscillations can be picked up here

// uniaxial things -- from ADC
static uint16_t uniaxial_adc;
static uint16_t uniaxial_v; // raw voltage
static int32_t uniaxial_g; // raw voltage
static uint16_t uniaxial_hz; // frequency

// triaxial things -- 16 bit sigs from SPI
static int16_t triaxial_x_g;
static int16_t triaxial_y_g;
static int16_t triaxial_z_g;

//static int16_t triaxial_x_raw;
//static int16_t triaxial_y_raw;
//static int16_t triaxial_z_raw;

//triaxial_x_g = ADXL372_GetG(triaxial_x_raw);
//triaxial_y_g = ADXL372_GetG(triaxial_y_raw);
//triaxial_z_g = ADXL372_GetG(triaxial_z_raw);

//uint16_t ADC_to_Voltage(uint16_t adc){
//    return (uint16_t)((uint32_t)adc * ADC_VREF) / ADC_MAX_VALUE;
//}

//float voltage_mv;

float offset_voltageG;
float g_force;

float strain_init;
float strain_corrected;
float offset_voltageS;

uint16_t ADC_to_Voltage(uint16_t adc){
    return (uint16_t)((uint32_t) (adc * 3600.0) / ADC_MAX_VALUE);
}

float Get_G_Force(uint16_t v) {
    // 1. Convert ADC to mV (Assumes 3.6V Range setting)
//    voltage_mv = (adc_raw * 3600.0f) / 4095.0f;

    // 2. Subtract Bias (1.7V = 1700mV)
    offset_voltageG = v - 1800.0f;

    // 3. Divide by Sensitivity (5mV per g)
    g_force = offset_voltageG / 5.0f;

    return g_force;
}

float Get_Strain(uint16_t v){



	offset_voltageS = v - 1250.0f;

	strain_init = (4 * offset_voltageS) / (GAUGE_FACTOR * EXCITATION);

	strain_corrected = strain_init / IA_GAIN;

	return strain_corrected;

}

// the triaxial accelerometer has a very large sensing range -- better for large shock forces rather than finer sensing
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
