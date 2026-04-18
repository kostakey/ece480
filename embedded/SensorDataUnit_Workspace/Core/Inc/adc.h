#ifndef __ADC_
#define __ADC_

#include "main.h"
#include "sensors.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim2;
uint16_t adc_buffer[2];

//extern uint16_t strain_gauge_adc;
//extern uint16_t uniaxial_adc;

//void ADC_Init(void){
//
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 2);
//	HAL_TIM_Base_Start(&htim2);
//
//}

//void Sample_ADCs(void){
//
//    HAL_ADC_Start(&hadc1);
//
//    // Wait for Strain Gauge (Rank 1)
//    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
//        strain_gauge_adc = (uint16_t) HAL_ADC_GetValue(&hadc1); // casted from 32 bit
//
//        // Wait for Piezo (Rank 2)
//		if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
//			uniaxial_adc = (uint16_t) HAL_ADC_GetValue(&hadc1);
//		}
//
//    }
//
//    HAL_ADC_Stop(&hadc1);
//
//}

void Sample_ADCs(void){
    // 1. Start the ADC
    HAL_ADC_Start(&hadc1);

    // 2. Poll for the FIRST channel (Rank 1 - Strain Gauge)
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        strain_gauge_adc = (uint32_t)HAL_ADC_GetValue(&hadc1);
    }

    // 3. Poll for the SECOND channel (Rank 2 - Piezo)
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uniaxial_adc = (uint32_t)HAL_ADC_GetValue(&hadc1);
    }

    // 4. Stop
    HAL_ADC_Stop(&hadc1);
}

#endif
