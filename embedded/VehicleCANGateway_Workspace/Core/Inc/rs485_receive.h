#ifndef __RECEIVERS485_
#define __RECEIVERS485_

#include "rs485.h"
#include "sensors.h"

void RS485bus_Read(void) {
    static uint8_t my_local_rx[8];
    static uint8_t index = 0;
    uint8_t rx_temp;
    HAL_StatusTypeDef status;

    // 1. Loop while data exists in the hardware buffer
//    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {

        status = HAL_UART_Receive(&huart1, &rx_temp, 1, 10);

        strain_gauge_hz = 1;

        if (status == HAL_OK) {
            if (index == 0) {
                // If we catch the start byte, begin the packet
                if (rx_temp == 0xA0 || rx_temp == 0xB0 || rx_temp == 0xC0) {
                    my_local_rx[index] = rx_temp;
                    index = 1;
                }
            } else {
                my_local_rx[index] = rx_temp;
                index++;

                if (index >= 8) {
                    // PROCESS DATA
                    if (my_local_rx[0] == 0xA0) {
                        // Debug: Prove we hit this block
                        uniaxial_hz = my_local_rx[0];

                        // Actual Data Mapping
                        triaxial_x_raw = (int16_t)((my_local_rx[2] << 8) | my_local_rx[1]);
                        triaxial_y_raw = (int16_t)((my_local_rx[4] << 8) | my_local_rx[3]);
                        triaxial_z_raw = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);
                    }
                    index = 0; // Reset for next packet
                }
            }
        } else if (status != HAL_TIMEOUT) {
            // 2. CRITICAL: Clear hardware errors (Overrun, Noise, etc.)
            // Without this, the UART will "freeze" if it sees a single bad bit.
            __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF | UART_CLEAR_PEF);
        }
//    }
}

//void RS485bus_Read(void) {
//    static uint8_t my_local_rx[8];
//    static uint8_t index = 0;
//    uint8_t rx_temp;
//
//    // We use a non-blocking timeout of 0 to poll the UART
//    HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, &rx_temp, 1, 0);
//
//    if (status == HAL_OK) {
//        if (index == 0) {
//            // Filter for any valid starting ID (A0, B0, or C0)
//            if (rx_temp == 0xA0 || rx_temp == 0xB0 || rx_temp == 0xC0) {
//                my_local_rx[index] = rx_temp;
//                index++;
//            }
//        } else {
//            // Collecting subsequent bytes
//            my_local_rx[index] = rx_temp;
//            index++;
//
//            // Once we hit our 8-byte packet size, process it
//            if (index >= 8) {
//                if (my_local_rx[0] == 0xA0) {
//                    // ID 0xA0: Triaxial Accelerometer Data
//                	uniaxial_hz = my_local_rx[0];
//
//                	triaxial_x_raw = (my_local_rx[2] << 8) | my_local_rx[1]; // Careful: check if your sender is Big or Little Endian
//                	triaxial_y_raw = (my_local_rx[4] << 8) | my_local_rx[3];
//                	triaxial_z_raw = (my_local_rx[6] << 8) | my_local_rx[5];
//
//                } else if (my_local_rx[0] == 0xB0) {
//                    // ID 0xB0: Strain Gauge Data
//                    strain_gauge_diff_v = (my_local_rx[3] << 8) | my_local_rx[2];
//                    strain_gauge_hz     = (my_local_rx[5] << 8) | my_local_rx[4];
//
//                } else if (my_local_rx[0] == 0xC0) {
//                    // ID 0xC0: Uniaxial Data
//                    uniaxial_v  = (my_local_rx[3] << 8) | my_local_rx[2];
//                    uniaxial_hz = (my_local_rx[5] << 8) | my_local_rx[4];
//                }
//
//                index = 0; // Reset index for the next packet
//            }
//        }
//    }
//}


#endif
