#ifndef __SENDRS485_
#define __SENDRS485_

#include "rs485.h"
#include "easytimer.h"

EasyTimer_t RS485Timer1 = {.start_time = 0, .interval = 50};  // 50ms

void Send_Test_RS485_Message(void) {
    RS485_Message rs_msg;

    rs_msg.start_byte = 0xAA;

    uint16_t bias = 12345;
    uint16_t travelFL = 12345;
    uint16_t travelFR = 12345;

    rs_msg.data[0] = (uint8_t)(bias & 0xFF);
    rs_msg.data[1] = (uint8_t)(bias >> 8);
    rs_msg.data[2] = (uint8_t)(travelFL & 0xFF);
    rs_msg.data[3] = (uint8_t)(travelFL >> 8);
    rs_msg.data[4] = (uint8_t)(travelFR & 0xFF);
    rs_msg.data[5] = (uint8_t)(travelFR >> 8);

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
}

void RS485_Online(void){

    if (Timer_HasElapsed(&RS485Timer1)) {
    	Send_Test_RS485_Message();
	}

}

#endif
