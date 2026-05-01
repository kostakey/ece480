#ifndef __RS485_
#define __RS485_

#include "main.h"

extern UART_HandleTypeDef huart1;

#define RS485_DATA_LEN 16 // Enough for Tri(6) + Strain(2) + Uniaxial(4) + Padding

typedef struct {
    uint8_t start_byte;
    uint8_t data[RS485_DATA_LEN];
    uint8_t checksum;
} RS485_Message;

/*
Packs the message struct into a transmission buffer, calculates an XOR checksum,
toggles the RS485 transceiver hardware pins, and sends the data over UART.
*/
void RS485_Write_Message(RS485_Message *msg, UART_HandleTypeDef *huart) {
    uint8_t tx_buf[18]; // 1 start + 16 data + 1 checksum

    tx_buf[0] = msg->start_byte;
    for(int i = 0; i < 16; i++) {
        tx_buf[i+1] = msg->data[i];
    }

    // Compute Checksum over Start + All Data
    uint8_t xor_sum = tx_buf[0];
    for(int i = 1; i < 17; i++) {
        xor_sum ^= tx_buf[i];
    }
    tx_buf[17] = xor_sum;

    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);

    // Transmit all 18 bytes
    HAL_UART_Transmit(huart, tx_buf, 18, 20);

    // CRITICAL: Ensure the last bit actually left the wire before switching back to RX
    while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);

    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
}

#endif
