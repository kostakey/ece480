#ifndef __RS485_
#define __RS485_

#include "main.h"

extern UART_HandleTypeDef huart1;

// Define the expected data length based on your Send function (6 bytes)
#define RS485_DATA_LEN 6

#define TX_QUEUE_SIZE 10

typedef struct {
    uint8_t start_byte; // e.g., 0xAA
    uint8_t data[6];    // 6 bytes of payload
    uint8_t checksum;   // Computed XOR byte
} RS485_Message;

//typedef enum {
//    STATE_WAIT_START,
//    STATE_COLLECT_DATA
//} RxState_t;

// --- Circular Buffer for Non-Halting TX ---
//typedef struct {
//    RS485_Message messages[TX_QUEUE_SIZE];
//    uint8_t head;
//    uint8_t tail;
//    uint8_t count;
//} RS485_Queue_t;

//static RS485_Queue_t txQueue = {0};
//static uint8_t tx_active_buf[8]; // Buffer for the current transfer

//void RS485_Enqueue(RS485_Message *msg) {
//    if (txQueue.count < TX_QUEUE_SIZE) {
//        txQueue.messages[txQueue.head] = *msg;
//        txQueue.head = (txQueue.head + 1) % TX_QUEUE_SIZE;
//        txQueue.count++;
//    }
//}

//void RS485_Process_Queue(UART_HandleTypeDef *huart) {
//    // Only start a new transmission if UART is free and queue isn't empty
//    if (huart->gState == HAL_UART_STATE_READY && txQueue.count > 0) {
//
//        RS485_Message *msg = &txQueue.messages[txQueue.tail];
//
//        // 1. Pack the static transfer buffer
//        tx_active_buf[0] = msg->start_byte;
//        uint8_t xor_sum = tx_active_buf[0];
//        for(int i = 0; i < 6; i++) {
//            tx_active_buf[i+1] = msg->data[i];
//            xor_sum ^= tx_active_buf[i+1];
//        }
//        tx_active_buf[7] = xor_sum;
//
//        // 2. Enable RS485 Driver
//        HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_SET);
//        HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_SET);
//
//        // 3. Start Non-Blocking Transmission
//        if (HAL_UART_Transmit_IT(huart, tx_active_buf, 8) == HAL_OK) {
//            txQueue.tail = (txQueue.tail + 1) % TX_QUEUE_SIZE;
//            txQueue.count--;
//        }
//    }
//}

// THIS IS CRITICAL: This callback runs when the last byte leaves the UART
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
//    if (huart->Instance == USART1) {
//        // Return to Receive Mode immediately
//        HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
//    }
//}

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
    HAL_UART_Transmit(huart, tx_buf, 8, 10); // 10ms to send 8 bytes on 9600baud

    // 5. WAIT for physical shifting to finish
//    while(__HAL_UART_GET_FLAG(huart, UART_FLAG_TC) == RESET);

    // 6. Return to Receive Mode
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
}

//int RS485_Read_Message(UART_HandleTypeDef *huart, RS485_Message *out_msg) {
//    static RxState_t state = STATE_WAIT_START;
//    static uint8_t data_index = 0;
//    uint8_t rx_byte;
//
//    // Check if a byte is available in the UART hardware (Non-blocking check)
//    // We use a timeout of 0 to ensure we don't stall the main loop
//    if (HAL_UART_Receive(huart, &rx_byte, 1, 0) == HAL_OK) {
//
//        switch (state) {
//            case STATE_WAIT_START:
//                if (rx_byte == 0xAA) {
//                    out_msg->start_byte = 0xAA;
//                    data_index = 0;
//                    state = STATE_COLLECT_DATA;
//                }
//                break;
//
//            case STATE_COLLECT_DATA:
//                out_msg->data[data_index++] = rx_byte;
//
//                if (data_index >= RS485_DATA_LEN) {
//                    state = STATE_WAIT_START; // Reset for next message
//                    return 1; // Success! Full message received
//                }
//                break;
//        }
//    }
//    return 0; // Message not yet complete
//}

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
