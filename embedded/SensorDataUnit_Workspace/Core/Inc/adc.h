#ifndef __ADC_
#define __ADC_

#include "main.h"
#include "sensors.h"

extern ADC_HandleTypeDef hadc1;

static uint16_t strain_gauge_adc;
static uint16_t uniaxial_adc;

void Sample_ADCs(void){

    HAL_ADC_Start(&hadc1);

    // Wait for Strain Gauge (Rank 1)
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        sensors.strain = HAL_ADC_GetValue(&hadc1);
    }

    // Wait for Piezo (Rank 2)
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        sensors.piezo = HAL_ADC_GetValue(&hadc1);
    }

    HAL_ADC_Stop(&hadc1);

}

#endif