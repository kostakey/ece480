/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32wl3x_hal.h"

#include "stm32wl3x_ll_bus.h"
#include "stm32wl3x_ll_cortex.h"
#include "stm32wl3x_ll_rcc.h"
#include "stm32wl3x_ll_system.h"
#include "stm32wl3x_ll_utils.h"
#include "stm32wl3x_ll_gpio.h"
#include "stm32wl3x_ll_pwr.h"
#include "stm32wl3x_ll_dma.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ACC_SPI_CS_Pin GPIO_PIN_0
#define CAN1_SPI_CS_Pin GPIO_PIN_0 // not used
#define ACC_SPI_CS_GPIO_Port GPIOA
#define CAN1_SPI_CS_GPIO_Port GPIOA // not used
#define CAN2_SPI_CS_Pin GPIO_PIN_9 // not used
#define CAN2_SPI_CS_GPIO_Port GPIOA // not used
#define RS485_Receiver_EN_Pin GPIO_PIN_12
#define RS485_Receiver_EN_GPIO_Port GPIOB
#define RS485_Driver_EN_Pin GPIO_PIN_13
#define RS485_Driver_EN_GPIO_Port GPIOB
#define CAN1_nINT_Pin GPIO_PIN_1 // not used
#define CAN1_nINT_GPIO_Port GPIOA // not used
#define CAN2_nINT_Pin GPIO_PIN_7 // not used
#define CAN2_nINT_GPIO_Port GPIOB // not used
#define Instrumentation_Amp_1_Pin GPIO_PIN_0
#define Instrumentation_Amp_1_GPIO_Port GPIOB
#define Instrumentation_Amp_2_Pin GPIO_PIN_1
#define Instrumentation_Amp_2_GPIO_Port GPIOB
#define Battery_Switch_Pin GPIO_PIN_2
#define Battery_Switch_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
