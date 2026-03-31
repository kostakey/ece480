#ifndef __TEMP_
#define __TEMP_

//#include "main.h"
//#include "rs485.h"

ADC_HandleTypeDef hadc1;

// Extern tells the compiler "this exists somewhere else, don't create a new one"
extern ADC_HandleTypeDef hadc1;
extern float global_celsius;

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef ConfigChannel = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.SequenceLength = 1;
  hadc1.Init.SamplingMode = ADC_SAMPLING_AT_START;
  hadc1.Init.SampleRate = ADC_SAMPLE_RATE_16;
  hadc1.Init.InvertOutputMode = ADC_DATA_INVERT_NONE;
  hadc1.Init.Overrun = ADC_NEW_DATA_IS_LOST;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DownSamplerConfig.DataWidth = ADC_DS_DATA_WIDTH_12_BIT;
  hadc1.Init.DownSamplerConfig.DataRatio = ADC_DS_RATIO_1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  ConfigChannel.Channel = ADC_CHANNEL_TEMPSENSOR;
  ConfigChannel.Rank = ADC_RANK_1;
  ConfigChannel.VoltRange = ADC_VIN_RANGE_1V2;
  ConfigChannel.CalibrationPoint.Number = ADC_CALIB_NONE;
  ConfigChannel.CalibrationPoint.Gain = 0x00;
  ConfigChannel.CalibrationPoint.Offset = 0x00;
  if (HAL_ADC_ConfigChannel(&hadc1, &ConfigChannel) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

#define TS_CAL1_ADDR ((uint16_t*) ((uint32_t)0x1FFF75A8)) // Calibrated at 30°C
#define TS_CAL2_ADDR ((uint16_t*) ((uint32_t)0x1FFF75CA)) // Calibrated at 110°C

void Broadcast_Temperature(void) {
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
        uint32_t adc_raw = HAL_ADC_GetValue(&hadc1);

        float ts_cal1 = (float)(*TS_CAL1_ADDR);
        float ts_cal2 = (float)(*TS_CAL2_ADDR);

        // Update the GLOBAL variable
        global_celsius = ((110.0f - 30.0f) / (ts_cal2 - ts_cal1))
                         * ((float)adc_raw - ts_cal1) + 30.0f;

        RS485_Send((uint8_t *)&global_celsius, 4);
    }
    HAL_ADC_Stop(&hadc1);
}

//void Broadcast_Temperature(void) {
//    uint32_t rawValue;
//    uint32_t celsius;
//
//    // 1. Start ADC, Wait for conversion, and Read
//    HAL_ADC_Start(&hadc1);
//    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
//        rawValue = HAL_ADC_GetValue(&hadc1);
//
//        // 2. Convert to Real Number (Simplified Example)
//        // Check your specific MCU datasheet for TS_CAL1/TS_CAL2 constants!
//        celsius = ((uint32_t)rawValue * 3.3f / 4095.0f - 0.76f) / 0.0025f + 25.0f;
//
//        // 3. Send the float over RS485
//        // We cast the float address to a uint8_t pointer
//        RS485_Send((uint8_t *)&celsius, sizeof(celsius));
//    }
//    HAL_ADC_Stop(&hadc1);
//}

#endif
