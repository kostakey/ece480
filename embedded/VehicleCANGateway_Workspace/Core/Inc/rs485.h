#ifndef __RS485_
#define __RS485_

#include "main.h"

UART_HandleTypeDef huart1;

// static void MX_USART1_UART_Init(void);

// void RS485_Send(uint8_t *pData, uint16_t Size);

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
//  huart1.Init.BaudRate = 115200;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
//  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

//void RS485_Send(uint8_t *pData, uint16_t Size) {
//    // 1. Switch to Transmit Mode
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);
//
//    // 2. Send Data
//    HAL_UART_Transmit(&huart1, pData, Size, 100);
//
//    // 3. IMPORTANT: Wait for "Transmission Complete" (TC) flag
//    // This ensures the last bit has physically left the wire.
//    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
//
//    // 4. Switch back to Receive Mode (Idle state)
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
//}

void RS485_Send(uint8_t *pData, uint16_t Size) {
    // 1. Enable Driver
    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);

    // 2. Send all 4 bytes
    HAL_UART_Transmit(&huart1, pData, Size, 1000); // Increased timeout to 1sec

    // 3. Wait for the hardware to actually finish
    // Check both TX Empty and Transmission Complete
    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);

    // 4. Tiny micro-delay (Safety for the transceiver chip)
    // At 9600 baud, 1 bit = 104us. Let's wait 200us.
    for(volatile int i=0; i<1000; i++);

    // 5. Disable Driver
    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
}
#endif
