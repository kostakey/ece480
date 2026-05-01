#ifndef __ADC_
#define __ADC_

#include "main.h"
#include "sensors.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
uint16_t adc_buffer[2];

/*
Manually triggers a scan of the configured ADC channels, polling for each conversion and storing the results in global variables.
*/
void Sample_ADCs(void){

    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        strain_gauge_adc = (uint32_t)HAL_ADC_GetValue(&hadc1);
    }

    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uniaxial_adc = (uint32_t)HAL_ADC_GetValue(&hadc1);
    }

    HAL_ADC_Stop(&hadc1);
}

#endif
