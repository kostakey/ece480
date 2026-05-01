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

// strain gauge things -- from ADC
static uint16_t strain_gauge_adc;
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

float offset_voltageG;
float g_force;

float strain_init;
float strain_corrected;
float offset_voltageS;

/*
Converts a raw 12-bit ADC value to a millivolt (mV) representation
based on a 3.6V scale.
*/
uint16_t ADC_to_Voltage(uint16_t adc){
    return (uint16_t)((uint32_t) (adc * 3600.0) / ADC_MAX_VALUE);
}

/*
Calculates G-force from a voltage input by removing a 1.8V offset
and applying the sensor's sensitivity scaling.
*/
float Get_G_Force(uint16_t v) {
    
    offset_voltageG = v - 1800.0f;

    g_force = offset_voltageG / 5.0f;

    return g_force;
}

/*
Converts voltage to strain using the Wheatstone bridge formula,
accounting for gauge factor, excitation voltage, and amplifier gain.
*/
float Get_Strain(uint16_t v){

	offset_voltageS = v - 1250.0f;

	strain_init = (4 * offset_voltageS) / (GAUGE_FACTOR * EXCITATION);

	strain_corrected = strain_init / IA_GAIN;

	return strain_corrected;

}

/*
Converts raw 12-bit signed data from the triaxial accelerometer
into a floating-point G-value.
*/
float Raw_to_G(int16_t raw_val) {
    if (raw_val & 0x0800) {
        raw_val |= 0xF000;
    } else {
        raw_val &= 0x0FFF;
    }


    return (float)raw_val * 0.1f;
}

#endif
