#ifndef __SENDCAN_
#define __SENDCAN_

#include "tcan.h"
#include "easytimer.h"
#include "sensors.h"

/*
Formats and transmits Triaxial Accelerometer data (X, Y, Z)
over CAN ID 0x900 with a 4-bit rolling counter.
*/
void Send_Message_900(uint16_t cs) {
    static uint8_t ctr = 0;
    TCAN_Message tx_msg;

    tx_msg.id = 900;
    tx_msg.len = 8;

    // Map your data to the buffer
    tx_msg.data[0] = ctr & 0x0F; // rolling counter stays between 0 and 15
    tx_msg.data[1] = 0;
    tx_msg.data[2] = (uint8_t)(triaxial_x_g & 0xFF);
    tx_msg.data[3] = (uint8_t)(triaxial_x_g >> 8);
    tx_msg.data[4] = (uint8_t)(triaxial_y_g & 0xFF);
    tx_msg.data[5] = (uint8_t)(triaxial_y_g >> 8);
    tx_msg.data[6] = (uint8_t)(triaxial_z_g & 0xFF);
    tx_msg.data[7] = (uint8_t)(triaxial_z_g >> 8);

    ctr++; // update rolling counter

    // Send it!
    TCAN4550_Write_Message(&tx_msg, cs);
}

/*
Formats and transmits Strain Gauge data over CAN ID 0x901
with a 4-bit rolling counter.
*/
void Send_Message_901(uint16_t cs) {
    static uint8_t ctr = 0;
    TCAN_Message tx_msg;

    tx_msg.id = 901;
    tx_msg.len = 8;

    // Map your data to the buffer
    tx_msg.data[0] = ctr & 0x0F; // rolling counter stays between 0 and 15
    tx_msg.data[1] = 0;
    tx_msg.data[2] = 0;
    tx_msg.data[3] = 0;
    tx_msg.data[4] = 0;
    tx_msg.data[5] = 0;
    tx_msg.data[6] = (uint8_t)(strain & 0xFF);
    tx_msg.data[7] = (uint8_t)(strain >> 8);

    ctr++; // update rolling counter

    // Send it!
    TCAN4550_Write_Message(&tx_msg, cs);
}

/*
Formats and transmits 32-bit Uniaxial G-force data over CAN ID 0x902
with a 4-bit rolling counter.
*/
void Send_Message_902(uint16_t cs) {
    static uint8_t ctr = 0;
    TCAN_Message tx_msg;

    tx_msg.id = 902;
    tx_msg.len = 8;

    // Map your data to the buffer
    tx_msg.data[0] = ctr & 0x0F; // rolling counter stays between 0 and 15
    tx_msg.data[1] = 0;
    tx_msg.data[2] = 0;
    tx_msg.data[3] = 0;
    tx_msg.data[4] = (uint8_t)(uniaxial_g & 0xFF);
    tx_msg.data[5] = (uint8_t)(uniaxial_g >> 8);
    tx_msg.data[6] = (uint8_t)(uniaxial_g >> 16);
    tx_msg.data[7] = (uint8_t)(uniaxial_g >> 24);

    ctr++; // update rolling counter

    // Send it!
    TCAN4550_Write_Message(&tx_msg, cs);
}

/*
Main CAN transmission loop that manages independent 10ms timers
for broadcasting sensor data frames.
*/
void CANbus_Send(uint16_t cs){

	static EasyTimer_t CANTimer900 = {.start_time = 0, .interval = 10};  // 10ms
    if (Timer_HasElapsed(&CANTimer900)) {
    	Send_Message_900(cs);

	}

    static EasyTimer_t CANTimer901 = {.start_time = 0, .interval = 10};  // 10ms
	if (Timer_HasElapsed(&CANTimer901)) {
		Send_Message_901(cs);

	}

    static EasyTimer_t CANTimer902 = {.start_time = 0, .interval = 10};  // 10ms
	if (Timer_HasElapsed(&CANTimer902)) {
		Send_Message_902(cs);

	}

}

#endif
