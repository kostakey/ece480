#ifndef __SENSORS_
#define __SENSORS_

#include "adxl.h"

#define ADC_BITS 12
#define ADC_VREF 3.3

// define sensor signals here, also create processing functions here -- shared between the gatewaya and the sensor board
// keep in mind, each signal is 16 bits

// need general ADC to raw voltage

// strain gauge things -- from ADC
extern static uint16_t strain_gauge_adc;
static uint16_t strain_gauge_plus_v;
static uint16_t strain_gauge_minus_adc;
static uint16_t strain_gauge_minus_v;
static uint16_t strain_gauge_diff_adc;
static uint16_t strain_gauge_diff_v;
static uint16_t strain_gauge_hz; // strain gauge frequency - see if oscillations can be picked up here

// uniaxial things -- from ADC
extern static uint16_t uniaxial_adc;
static uint16_t uniaxial_v; // raw voltage
static uint16_t uniaxial_hz; // frequency

// triaxial things -- 16 bit sigs from SPI
extern static uint16_t triaxial_x_g;
extern static uint16_t triaxial_y_g;
extern static uint16_t triaxial_z_g;

uint16_t ADC_to_Voltage(uint16_t adc){

    return ;
}


uint16_t Uniaxial_Voltage_To_Freq(uint16_t v){

    return;
}

uint16_t Strain_Gauge_Diff_To_Freq(uint16_t v){

    return;
}

#endif
