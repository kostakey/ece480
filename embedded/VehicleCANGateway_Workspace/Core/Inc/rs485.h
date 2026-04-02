#ifndef __RS485_
#define __RS485_

#include "main.h"

extern UART_HandleTypeDef huart1;

// void RS485_Send(uint8_t *pData, uint16_t Size);



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

//RS485_Driver_EN_Pin

//void RS485_Send(uint8_t *pData, uint16_t Size) {
//    // 1. Enable Driver
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);
//
//    // 2. Send all 4 bytes
//    HAL_UART_Transmit(&huart1, pData, Size, 1000); // Increased timeout to 1sec
//
//    // 3. Wait for the hardware to actually finish
//    // Check both TX Empty and Transmission Complete
//    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET);
//
//    // 4. Tiny micro-delay (Safety for the transceiver chip)
//    // At 9600 baud, 1 bit = 104us. Let's wait 200us.
//    for(volatile int i=0; i<1000; i++);
//
//    // 5. Disable Driver
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
//}

void RS485_Send(uint8_t *pData, uint16_t Size) {
    // 1. Enable Driver (Disable Receiver to avoid echo)
    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);

    // 2. Clear TC flag to ensure we wait for THIS transmission
    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);

    // 3. Send data
    HAL_UART_Transmit(&huart1, pData, Size, 1000);

    // 4. Wait for hardware to physically finish shifting out the last bit
//    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET) {
//        // Optional: add a tiny timeout here for safety
//    }

    // 5. Final hardware "settling" delay
    // If you have a DWT delay function, use it. Otherwise, a few NOPs:
//    for(volatile int i=0; i<500; i++) { __NOP(); }

    // 6. Return to Receive Mode
    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
}

//void RS485_Send(uint8_t *pData, uint16_t Size) {
//    // 1. Enter Transmit Mode
//    // DE = High (Enable Driver), RE = High (Disable Receiver)
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);
//
//    // 2. Clear the Transmission Complete (TC) flag
//    // This ensures we don't catch a "stale" flag from a previous send
//    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);
//
//    // 3. Start the blocking transmit
//    HAL_UART_Transmit(&huart1, pData, Size, 1000);
//
//    // 4. Wait for the PHYSICAL last bit to leave the pin
//    // HAL_UART_Transmit returns when the last byte is in the shift register,
//    // NOT necessarily when it has finished being pulsed out.
//    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET) {
//        // Optional: Add a small timeout counter here if you're paranoid
//    }
//
//    // 5. Mandatory Guard Time
//    // Even after TC is set, some transceivers need ~1/2 bit time to settle.
//    // At 9600 baud, 1 bit = ~104us.
//    // On WL33 (M0+), this loop is roughly 150-200us depending on clock speed.
//    for(volatile int i = 0; i < 1000; i++) {
//        __NOP();
//    }
//
//    // 6. Return to Receive Mode
//    // DE = Low, RE = Low
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
//}

void RS485_Send_Test_Message(uint8_t *pData) {
    uint8_t tx_buf[8];

    tx_buf[0] = 0xAA;	// Start of Frame
    tx_buf[1] = pData[0];
    tx_buf[2] = pData[1];
    tx_buf[3] = pData[2];
    tx_buf[4] = pData[3];
    tx_buf[5] = pData[4];
	tx_buf[6] = pData[5];

    // Simple XOR Checksum (Low Overhead)
    uint8_t checksum = 0;
    for(int i = 0; i < 5; i++) {
        checksum ^= tx_buf[i];
    }
    tx_buf[7] = checksum;

    // Send it!
    RS485_Send(tx_buf, 8);
}

#endif
