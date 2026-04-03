#ifndef __SENDCAN_
#define __SENDCAN_

#include "tcan.h"
#include "easytimer.h"

EasyTimer_t CANTimer123 = {.start_time = 0, .interval = 100};  // 500ms
EasyTimer_t CANTimer600 = {.start_time = 0, .interval = 100};  // 500ms

void Send_Test_CAN_Message(uint16_t cs) {
    static uint8_t ctr = 0;
    TCAN_Message tx_msg;

    tx_msg.id = 123;
    tx_msg.len = 8;

    uint16_t bias = 12345;
    uint16_t travelFL = 12345;
    uint16_t travelFR = 12345;

    // Map your data to the buffer
    tx_msg.data[0] = ctr & 0x0F; // rolling counter stays between 0 and 15
    tx_msg.data[1] = 0;
    tx_msg.data[2] = (uint8_t)(bias & 0xFF);
    tx_msg.data[3] = (uint8_t)(bias >> 8);
    tx_msg.data[4] = (uint8_t)(travelFL & 0xFF);
    tx_msg.data[5] = (uint8_t)(travelFL >> 8);
    tx_msg.data[6] = (uint8_t)(travelFR & 0xFF);
    tx_msg.data[7] = (uint8_t)(travelFR >> 8);

    ctr++; // update rolling counter

    // Send it!
    TCAN4550_Write_Message(&tx_msg, cs);
}

void CAN1_Online(uint16_t cs){

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
