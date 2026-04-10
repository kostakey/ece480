#ifndef __SENDCAN_
#define __SENDCAN_

#include "tcan.h"
#include "easytimer.h"
#include "sensors.h"

// define all CAN timers here

//EasyTimer_t CANTimer600 = {.start_time = 0, .interval = 100};  // 500ms

void Send_Test_CAN_Message(uint16_t cs) {
    static uint8_t ctr = 0;
    TCAN_Message tx_msg;

    tx_msg.id = 123;
    tx_msg.len = 8;

    // Map your data to the buffer
    tx_msg.data[0] = ctr & 0x0F; // rolling counter stays between 0 and 15
    tx_msg.data[1] = 0;
    tx_msg.data[2] = (uint8_t)(triaxial_x_raw & 0xFF);
    tx_msg.data[3] = (uint8_t)(triaxial_x_raw >> 8);
    tx_msg.data[4] = (uint8_t)(triaxial_y_raw & 0xFF);
    tx_msg.data[5] = (uint8_t)(triaxial_y_raw >> 8);
    tx_msg.data[6] = (uint8_t)(triaxial_z_raw & 0xFF);
    tx_msg.data[7] = (uint8_t)(triaxial_z_raw >> 8);

    ctr++; // update rolling counter

    // Send it!
    TCAN4550_Write_Message(&tx_msg, cs);
}

void CANbus_Send(uint16_t cs){

	static EasyTimer_t CANTimer123 = {.start_time = 0, .interval = 50};  // 50ms
    if (Timer_HasElapsed(&CANTimer123)) {
		  Send_Test_CAN_Message(cs);

	}

//    if (Timer_HasElapsed(&CANTimer601)) {
//    		  Send_Test_CAN_Message(cs);
//    }

}

// void CAN2_Online(void){

//     if (Timer_HasElapsed(&CAN2Timer)) {
// 		  Send_Test_RS485_Message();
// 	}

// }

#endif
