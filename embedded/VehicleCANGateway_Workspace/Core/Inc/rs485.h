#ifndef __RS485_
#define __RS485_

#include "main.h"

extern UART_HandleTypeDef huart1;

// Define the expected data length based on your Send function (6 bytes)
#define RS485_DATA_LEN 6

typedef struct {
    uint8_t start_byte; // e.g., 0xAA
    uint8_t data[6];    // 6 bytes of payload
    uint8_t checksum;   // Computed XOR byte
} RS485_Message;

typedef enum {
    RX_STATE_IDLE,
    RX_STATE_DATA,
    RX_STATE_CHECKSUM
} RS485_RxState_t;

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

int RS485_Read_Message(UART_HandleTypeDef *huart, RS485_Message *out_msg) {
    static RS485_RxState_t state = RX_STATE_IDLE;
    static uint8_t data_idx = 0;
    static uint8_t calc_xor = 0;
    uint8_t rx_byte;

    // Non-blocking poll: Check if a byte is sitting in the UART hardware
    if (HAL_UART_Receive(huart, &rx_byte, 1, 0) == HAL_OK) {

        switch (state) {
            case RX_STATE_IDLE:
                // Look for any of your valid start bytes (A0, B0, or C0)
                if (rx_byte == 0xA0 || rx_byte == 0xB0 || rx_byte == 0xC0) {
                    out_msg->start_byte = rx_byte;
                    calc_xor = rx_byte; // Start checksum with the ID
                    data_idx = 0;
                    state = RX_STATE_DATA;
                }
                break;

            case RX_STATE_DATA:
                out_msg->data[data_idx] = rx_byte;
                calc_xor ^= rx_byte; // Accumulate XOR checksum
                data_idx++;

                if (data_idx >= 6) {
                    state = RX_STATE_CHECKSUM;
                }
                break;

            case RX_STATE_CHECKSUM:
                out_msg->checksum = rx_byte;
                state = RX_STATE_IDLE; // Reset for next packet

                // Validate Checksum
                if (out_msg->checksum == calc_xor) {
                    return 1; // SUCCESS: Full valid message received
                } else {
                    // Checksum error - discard packet
                    return -1;
                }
                break;
        }
    }
    return 0; // Still waiting for more bytes
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
