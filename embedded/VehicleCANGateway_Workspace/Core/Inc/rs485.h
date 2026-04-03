#ifndef __RS485_
#define __RS485_

#include "main.h"

extern UART_HandleTypeDef huart1;

typedef struct {
    uint8_t start_byte; // e.g., 0xAA
    uint8_t data[6];    // 6 bytes of payload
    uint8_t checksum;   // Computed XOR byte
} RS485_Message;

//void RS485_Send(uint8_t *pData, uint16_t Size) {
//    // 1. Enable Driver (Disable Receiver to avoid echo)
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);
//
//    // 2. Clear TC flag to ensure we wait for THIS transmission
//    __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_TC);
//
//    // 3. Send data
//    HAL_UART_Transmit(&huart1, pData, Size, 1000);
//
//    // 4. Wait for hardware to physically finish shifting out the last bit
////    while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET) {
////        // Optional: add a tiny timeout here for safety
////    }
//
//    // 5. Final hardware "settling" delay
//    // If you have a DWT delay function, use it. Otherwise, a few NOPs:
////    for(volatile int i=0; i<500; i++) { __NOP(); }
//
//    // 6. Return to Receive Mode
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
//}

void RS485_Write_Message(RS485_Message *msg, UART_HandleTypeDef *huart) {
    uint8_t tx_buf[8];

    // 1. Pack the buffer
    tx_buf[0] = msg->start_byte;
    for(int i = 0; i < 6; i++) {
        tx_buf[i+1] = msg->data[i];
    }

    // 2. Compute Checksum (XOR of Start + Data)
    uint8_t xor_sum = tx_buf[0];
    for(int i = 1; i < 7; i++) {
        xor_sum ^= tx_buf[i];
    }
    tx_buf[7] = xor_sum;

    // 3. Enable Driver / Disable Receiver
    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);

    // 4. Send via UART
    HAL_UART_Transmit(huart, tx_buf, 8, 100);

    // 5. WAIT for physical shifting to finish
    while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);

    // 6. Return to Receive Mode
    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
}

//void RS485_Send_Test_Message(uint8_t *pData) {
//    uint8_t tx_buf[8];
//
//    tx_buf[0] = 0xAA;	// Start of Frame
//    tx_buf[1] = pData[0];
//    tx_buf[2] = pData[1];
//    tx_buf[3] = pData[2];
//    tx_buf[4] = pData[3];
//    tx_buf[5] = pData[4];
//	tx_buf[6] = pData[5];
//
//    // Simple XOR Checksum (Low Overhead)
//    uint8_t checksum = 0;
//    for(int i = 0; i < 5; i++) {
//        checksum ^= tx_buf[i];
//    }
//    tx_buf[7] = checksum;
//
//    // Send it!
//    RS485_Send(tx_buf, 8);
//}

#endif
