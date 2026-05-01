#ifndef __RECEIVERS485_
#define __RECEIVERS485_

#include "rs485.h"
#include "sensors.h"

/*
Non-blocking UART parser that synchronizes to the 0xD0 start byte and
reconstructs the 18-byte "Super Packet" into global sensor variables.
*/
void RS485bus_Read(void) {
    uint8_t rx_temp;
    HAL_StatusTypeDef status;
    static uint8_t index = 0;
    static uint8_t my_local_rx[18];

    // 1. Clear hardware lockups (Overrun/Noise) before starting
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) || __HAL_UART_GET_FLAG(&huart1, UART_FLAG_NE)) {
        __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF);
    }

    // 2. Drain the hardware buffer as fast as possible
    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
        // 0 timeout ensures the loop doesn't block other tasks (like CAN)
        status = HAL_UART_Receive(&huart1, &rx_temp, 1, 0);

        if (status == HAL_OK) {
            if (index == 0) {
                // Look for the "Super Packet" Start Byte
                if (rx_temp == 0xD0) {
                    my_local_rx[index] = rx_temp;
                    index = 1;
                }
            } else {
                my_local_rx[index] = rx_temp;
                index++;

                // If we have the full packet (1 Start + 16 Data + 1 Checksum-slot)
                if (index >= 18) {

                    // 3. MAPPING DATA (NO CHECKSUM GATE)

                    // Triaxial X, Y, Z (Bytes 1-6)
                    triaxial_x_g = (int16_t)((my_local_rx[2] << 8) | my_local_rx[1]);
                    triaxial_y_g = (int16_t)((my_local_rx[4] << 8) | my_local_rx[3]);
                    triaxial_z_g = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);

                    // Strain Gauge (Bytes 7-8)
                    strain = (int16_t)((my_local_rx[8] << 8) | my_local_rx[7]);

                    // Uniaxial G-Force (Bytes 9-12)
                    uniaxial_g = (int32_t)( ((uint32_t)my_local_rx[12] << 24) |
                                 ((uint32_t)my_local_rx[11] << 16) |
                                 ((uint32_t)my_local_rx[10] << 8)  |
                                 ((uint32_t)my_local_rx[9]) );

                    index = 0; // Reset for next message
                }
            }
        }
    }
}

#endif
