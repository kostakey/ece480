#ifndef __RECEIVERS485_
#define __RECEIVERS485_

#include "rs485.h"
#include "sensors.h"

// do the receive parsing here
//void RS485bus_Read(void) {
//    RS485_Message incoming;
//
//    if (RS485_Read_Message(&huart1, &incoming)) {
//
//        // use the start byte to classify each message
//        // if data[i] == designator that tells me which CAN message to pack...
//        // Reassemble the 16-bit values (Little Endian as per your sender)
//        bias     = (uint16_t)(incoming.data[0] | (incoming.data[1] << 8));
//        travelFL = (uint16_t)(incoming.data[2] | (incoming.data[3] << 8));
//        travelFR = (uint16_t)(incoming.data[4] | (incoming.data[5] << 8));
//
//        // Now do something with the data...
////        printf("Received Bias: %u, FL: %u, FR: %u\n", rx_bias, rx_travelFL, rx_travelFR);
//    }
//}

void RS485bus_Read(void){
	RS485_Message rx_buffer;

	if (RS485_Read_Message(&huart1, &rx_buffer) == 1) {

		// Use a switch to handle different message IDs
		switch (rx_buffer.start_byte) {
			case 0xA0:
				triaxial_x_g = (rx_buffer.data[1] << 8) | rx_buffer.data[0];
				triaxial_y_g = (rx_buffer.data[3] << 8) | rx_buffer.data[2];
				triaxial_z_g = (rx_buffer.data[5] << 8) | rx_buffer.data[4];
				break;
			case 0xB0:
				strain_gauge_diff_v = (rx_buffer.data[3] << 8) | rx_buffer.data[2];
				strain_gauge_hz     = (rx_buffer.data[5] << 8) | rx_buffer.data[4];
				break;
			case 0xC0:
				uniaxial_v  = (rx_buffer.data[3] << 8) | rx_buffer.data[2];
				uniaxial_hz = (rx_buffer.data[5] << 8) | rx_buffer.data[4];
				break;
		}
	}

}


#endif
