#ifndef __ADC_
#define __ADC_

#include "main.h"
#include "sensors.h"

extern ADC_HandleTypeDef hadc1;

//extern uint16_t strain_gauge_adc;
//extern uint16_t uniaxial_adc;

void Sample_ADCs(void){

    HAL_ADC_Start(&hadc1);

    // Wait for Strain Gauge (Rank 1)
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        strain_gauge_adc = (uint16_t) HAL_ADC_GetValue(&hadc1); // casted from 32 bit

        // Wait for Piezo (Rank 2)
		if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
			uniaxial_adc = (uint16_t) HAL_ADC_GetValue(&hadc1);
		}

    }

    HAL_ADC_Stop(&hadc1);

}

#endif
