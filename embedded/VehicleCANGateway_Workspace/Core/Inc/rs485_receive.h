#ifndef __RECEIVERS485_
#define __RECEIVERS485_

#include "rs485.h"
#include "sensors.h"

static uint8_t byte0;
static uint8_t byte1;
static uint8_t byte2;
static uint8_t byte3;
static uint8_t byte4;
static uint8_t byte5;
static uint8_t byte6;
static uint8_t byte7;
static uint8_t byte8;
static uint8_t byte9;
static uint8_t byte10;
static uint8_t byte11;
static uint8_t byte12;
static uint8_t byte13;
static uint8_t byte14;
static uint8_t byte15;
static uint8_t byte16;
static uint8_t byte17;


//void RS485bus_Read(void) {
//    uint8_t rx_temp;
//    HAL_StatusTypeDef status;
//    static uint8_t index = 0;
//    static uint8_t my_local_rx[18];
//
//    // 1. Clear hardware errors (Overrun/Noise)
//    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) || __HAL_UART_GET_FLAG(&huart1, UART_FLAG_NE)) {
//        __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF);
//    }
//
//    // 2. Process all bytes currently waiting in the UART FIFO
//    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
//        status = HAL_UART_Receive(&huart1, &rx_temp, 1, 0);
//
//        if (status == HAL_OK) {
//            if (index == 0) {
//                // Synchronize on the Start Byte
//                if (rx_temp == 0xD0) {
//                    my_local_rx[index] = rx_temp;
//                    index = 1;
//                }
//            } else {
//                my_local_rx[index] = rx_temp;
//                index++;
//
//                // Once we have the full 18-byte packet
//                if (index >= 18) {
//
//                    // 3. CHECKSUM VERIFICATION
//                    // Calculate XOR of bytes 0 through 16
//                    uint8_t calculated_xor = 0;
//                    for (int i = 0; i < 17; i++) {
//                        calculated_xor ^= my_local_rx[i];
//                    }
//
//                    // Only map data if the calculated XOR matches the received byte (index 17)
//                    if (calculated_xor == my_local_rx[17]) {
//
//                        // Increment this to prove "Good" packets are arriving
//                        strain_gauge_hz++;
//
//                        // 4. MAPPING VALIDATED DATA
//                        // Triaxial X, Y, Z (Bytes 1-6)
//                        triaxial_x_g = (int16_t)((my_local_rx[2] << 8) | my_local_rx[1]);
//                        triaxial_y_g = (int16_t)((my_local_rx[4] << 8) | my_local_rx[3]);
//                        triaxial_z_g = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);
//
//                        // Strain Gauge (Bytes 7-8)
//                        strain_gauge_diff_v = (uint16_t)((my_local_rx[8] << 8) | my_local_rx[7]);
//
//                        // Uniaxial G-Force (Bytes 9-12)
//                        uniaxial_g = (int32_t)( ((uint32_t)my_local_rx[12] << 24) |
//                                                ((uint32_t)my_local_rx[11] << 16) |
//                                                ((uint32_t)my_local_rx[10] << 8)  |
//                                                ((uint32_t)my_local_rx[9]) );
//                    }
//
//                    // Reset index to look for the next 0xD0
//                    index = 0;
//                }
//            }
//        }
//    }
//}

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

//                	byte0 = my_local_rx[0];
//                	byte1 = my_local_rx[1];
//                	byte2 = my_local_rx[2];
//                	byte3 = my_local_rx[3];
//                	byte4 = my_local_rx[4];
//                	byte5 = my_local_rx[5];
//                	byte6 = my_local_rx[6];
//                	byte7 = my_local_rx[7];
//                	byte8 = my_local_rx[8];
//                	byte9 = my_local_rx[9];
//                	byte10 = my_local_rx[10];
//                	byte11 = my_local_rx[11];
//                	byte12 = my_local_rx[12];
//                	byte13 = my_local_rx[13];
//                	byte14 = my_local_rx[14];
//                	byte15 = my_local_rx[15];
//                	byte16 = my_local_rx[16];
//                	byte17 = my_local_rx[17];

                    // 3. MAPPING DATA (NO CHECKSUM GATE)
//                    strain_gauge_hz++; // Counting every time we fill 18 bytes

                    // Triaxial X, Y, Z (Bytes 1-6)
                    triaxial_x_g = (int16_t)((my_local_rx[2] << 8) | my_local_rx[1]);
                    triaxial_y_g = (int16_t)((my_local_rx[4] << 8) | my_local_rx[3]);
                    triaxial_z_g = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);

                    // Strain Gauge (Bytes 7-8)
                    strain_gauge_diff_v = (uint16_t)((my_local_rx[8] << 8) | my_local_rx[7]);

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

//void RS485bus_Read(void) {
//    uint8_t rx_temp;
//    HAL_StatusTypeDef status;
//    static uint8_t index = 0;
//    static uint8_t my_local_rx[18];
//
//    // 1. Clear hardware lockups (Overrun/Noise) before starting
//    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) || __HAL_UART_GET_FLAG(&huart1, UART_FLAG_NE)) {
//        __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF);
//    }
//
//    // 2. Read all available bytes in the hardware buffer
//    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
//        // Use 0 timeout to ensure the loop stays fast
//        status = HAL_UART_Receive(&huart1, &rx_temp, 1, 0);
//
//        if (status == HAL_OK) {
//            if (index == 0) {
//                // Look for the "Super Packet" Start Byte (0xD0)
//                if (rx_temp == 0xD0) {
//                    my_local_rx[index] = rx_temp;
//                    index = 1;
//                    strain_gauge_hz++; // Debug counter for hits
//                }
//            } else {
//                my_local_rx[index] = rx_temp;
//                index++;
//
//                // If we have reached 18 bytes (1 start + 16 data + 1 old-checksum-slot)
//                if (index >= 18) {
//
//                    // 3. MAPPING DATA
//                    // Triaxial X, Y, Z (Bytes 1-6)
//                    triaxial_x_g = (int16_t)((my_local_rx[2] << 8) | my_local_rx[1]);
//                    triaxial_y_g = (int16_t)((my_local_rx[4] << 8) | my_local_rx[3]);
//                    triaxial_z_g = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);
//
//                    // Strain Gauge (Bytes 7-8)
//                    strain_gauge_diff_v = (uint16_t)((my_local_rx[8] << 8) | my_local_rx[7]);
//
//                    // Uniaxial G-Force (Bytes 9-12)
//                    uniaxial_g = (int32_t)( ((uint32_t)my_local_rx[12] << 24) |
//                                            ((uint32_t)my_local_rx[11] << 16) |
//                                            ((uint32_t)my_local_rx[10] << 8)  |
//                                            ((uint32_t)my_local_rx[9]) );
//
//                    index = 0; // Reset for next message
//                }
//            }
//        }
//    }
//}

//void RS485bus_Read(void) {
//    uint8_t rx_temp;
//    HAL_StatusTypeDef status;
//    static uint8_t index = 0;
//    static uint8_t my_local_rx[18];
//
//    // Ensure we are in Receive Mode (DE/RE Low)
//    HAL_GPIO_WritePin(GPIOB, RS485_Driver_EN_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin(GPIOB, RS485_Receiver_EN_Pin, GPIO_PIN_RESET);
//
//    // 1. Clear any existing errors before starting the read
//    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE) || __HAL_UART_GET_FLAG(&huart1, UART_FLAG_NE)) {
//        __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF);
//    }
//
//    // 2. Drain the hardware buffer as fast as possible
//    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
//        status = HAL_UART_Receive(&huart1, &rx_temp, 1, 10); // 0 timeout for speed
//
//        if (status == HAL_OK) {
//            if (index == 0) {
//                // Look for the "Super Packet" Start Byte
//                if (rx_temp == 0xD0) {
//                    my_local_rx[index] = rx_temp;
//                    index = 1;
//                    strain_gauge_hz++;
//                }
//            } else {
//                my_local_rx[index] = rx_temp;
//                index++;
////                strain_gauge_hz = 0;
//                // If we have the full packet (1 Start + 16 Data + 1 Checksum)
//                if (index >= 18) {
//                    // 3. VERIFY CHECKSUM
//                    uint8_t calculated_xor = my_local_rx[0]; // Start with the Start Byte
//                    for (int i = 1; i < 17; i++) {
//                        calculated_xor ^= my_local_rx[i];
//                    }
//
//                    // Compare calculated vs received (last byte)
//                    if (calculated_xor == my_local_rx[17]) {
//
//                        // 4. MAPPING DATA (Matches Send_Combined_Message)
//
//                        // Triaxial X, Y, Z (Bytes 1-6)
//                        triaxial_x_g = (int16_t)((my_local_rx[2] << 8) | my_local_rx[1]);
//                        triaxial_y_g = (int16_t)((my_local_rx[4] << 8) | my_local_rx[3]);
//                        triaxial_z_g = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);
//
//                        // Strain Gauge (Bytes 7-8)
//                        strain_gauge_diff_v = (uint16_t)((my_local_rx[8] << 8) | my_local_rx[7]);
//
//                        // Uniaxial G-Force (Bytes 9-12)
//                        uniaxial_g = (int32_t)( ((uint32_t)my_local_rx[12] << 24) |
//                                                ((uint32_t)my_local_rx[11] << 16) |
//                                                ((uint32_t)my_local_rx[10] << 8)  |
//                                                ((uint32_t)my_local_rx[9]) );
//                    }
//
//                    index = 0; // Reset for next message
//                }
//            }
//        }
//    }
//}

//void RS485bus_Read(void) {
//    static uint8_t my_local_rx[8];
//    static uint8_t index = 0;
//    uint8_t rx_temp;
//    HAL_StatusTypeDef status;
//
//    // 1. Loop while data exists in the hardware buffer
////    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
//
//        status = HAL_UART_Receive(&huart1, &rx_temp, 1, 10);
//
//        if (status == HAL_OK) {
//            if (index == 0) {
//                // If we catch the start byte, begin the packet
//                if (rx_temp == 0xA0 || rx_temp == 0xB0 || rx_temp == 0xC0) {
//                    my_local_rx[index] = rx_temp;
//                    index = 1;
//                }
//            } else {
//                my_local_rx[index] = rx_temp;
//                index++;
//
//                if (index >= 8) {
//                    // PROCESS DATA
//                    if (my_local_rx[0] == 0xA0) {
//                        // Actual Data Mapping
//                        triaxial_x_raw = (int16_t)((my_local_rx[2] << 8) | my_local_rx[1]);
//                        triaxial_y_raw = (int16_t)((my_local_rx[4] << 8) | my_local_rx[3]);
//                        triaxial_z_raw = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);
//                    } else if (my_local_rx[0] == 0xB0) {
//                    	strain_gauge_diff_v = (int16_t)((my_local_rx[6] << 8) | my_local_rx[5]);
//                    } else if (my_local_rx[0] == 0xC0) {
//                    	uniaxial_v = (int32_t)( ((uint32_t)my_local_rx[6] << 24) |
//                    	                        ((uint32_t)my_local_rx[5] << 16) |
//                    	                        ((uint32_t)my_local_rx[4] << 8)  |
//                    	                        ((uint32_t)my_local_rx[3]) );
//                    }
//
//                    index = 0; // Reset for next packet
//                }
//            }
//        } else if (status != HAL_TIMEOUT) {
//            // 2. CRITICAL: Clear hardware errors (Overrun, Noise, etc.)
//            // Without this, the UART will "freeze" if it sees a single bad bit.
//            __HAL_UART_CLEAR_IT(&huart1, UART_CLEAR_OREF | UART_CLEAR_NEF | UART_CLEAR_FEF | UART_CLEAR_PEF);
//        }
////    }
//}

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
