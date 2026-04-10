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

//triaxial_x_g = ADXL372_GetG(triaxial_x_raw);
//triaxial_y_g = ADXL372_GetG(triaxial_y_raw);
//triaxial_z_g = ADXL372_GetG(triaxial_z_raw);

uint16_t ADC_to_Voltage(uint16_t adc){
    return (uint16_t)((uint32_t)adc * ADC_VREF) / ADC_MAX_VALUE;
}

uint16_t Raw_to_G(uint16_t raw){
	return ADXL372_GetG(raw);
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
