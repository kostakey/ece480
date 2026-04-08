#ifndef __SENDRS485_
#define __SENDRS485_

#include "rs485.h"
#include "easytimer.h"
#include "sensors.h"
#include "adc.h"

// define all RS485 timers here, timing limited by baud rate
// EasyTimer_t RS485TimerTest = {.start_time = 0, .interval = 50};  // 50ms
//EasyTimer_t RS485TimerA0 = {.start_time = 0, .interval = 50};  // 50ms
//EasyTimer_t RS485TimerB0 = {.start_time = 0, .interval = 50};  // 50ms
//EasyTimer_t RS485TimerC0 = {.start_time = 0, .interval = 50};  // 50ms

// void Send_Test_RS485_Message(void) {
//     RS485_Message rs_msg;

//     rs_msg.start_byte = 0xAA; // id, used to identify what goes where later on

//     // bias = 12345;
//     // uint16_t travelFL = 12345;
//     // uint16_t travelFR = 12345;

//     rs_msg.data[0] = (uint8_t)(bias & 0xFF);
//     rs_msg.data[1] = (uint8_t)(bias >> 8);
//     rs_msg.data[2] = (uint8_t)(travelFL & 0xFF);
//     rs_msg.data[3] = (uint8_t)(travelFL >> 8);
//     rs_msg.data[4] = (uint8_t)(travelFR & 0xFF);
//     rs_msg.data[5] = (uint8_t)(travelFR >> 8);

//     // Send it using UART1 handle
//     RS485_Write_Message(&rs_msg, &huart1);
// }

void Send_RS485_Message_A0(void) {
    RS485_Message rs_msg;

    // rs_msg.start_byte = 0xAA;
    rs_msg.start_byte = 0xA0; // id for triaxial x, y, and z msg

    // 16 bits per sig

//    rs_msg.data[0] = (uint8_t)(triaxial_x_g & 0xFF);
//    rs_msg.data[1] = (uint8_t)(triaxial_x_g >> 8);
//    rs_msg.data[2] = (uint8_t)(triaxial_y_g & 0xFF);
//    rs_msg.data[3] = (uint8_t)(triaxial_y_g >> 8);
//    rs_msg.data[4] = (uint8_t)(triaxial_z_g & 0xFF);
//    rs_msg.data[5] = (uint8_t)(triaxial_z_g >> 8);

    rs_msg.data[0] = 0xA0;
	rs_msg.data[1] = 0xA0;
	rs_msg.data[2] = 0xA0;
	rs_msg.data[3] = 0xA0;
	rs_msg.data[4] = 0xA0;
	rs_msg.data[5] = 0xA0;

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
//    RS485_Enqueue(&rs_msg);
}

void Send_RS485_Message_B0(void) {
    RS485_Message rs_msg;

    // rs_msg.start_byte = 0xAA;
    rs_msg.start_byte = 0xB0; // id for strain gauge diff and freq msg

    strain_gauge_diff_v = ADC_to_Voltage(strain_gauge_adc);
    // strain_gauge_hz = v to hz function

//    rs_msg.data[0] = 0;
//    rs_msg.data[1] = 0;
//    rs_msg.data[2] = (uint8_t)(strain_gauge_diff_v & 0xFF);
//    rs_msg.data[3] = (uint8_t)(strain_gauge_diff_v >> 8);
//    rs_msg.data[4] = (uint8_t)(strain_gauge_hz & 0xFF);
//    rs_msg.data[5] = (uint8_t)(strain_gauge_hz >> 8);

    rs_msg.data[0] = 0xB0;
    rs_msg.data[1] = 0xB0;
    rs_msg.data[2] = 0xB0;
    rs_msg.data[3] = 0xB0;
    rs_msg.data[4] = 0xB0;
    rs_msg.data[5] = 0xB0;

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
//    RS485_Enqueue(&rs_msg);
}

void Send_RS485_Message_C0(void) {
    RS485_Message rs_msg;

    // rs_msg.start_byte = 0xAA;
    rs_msg.start_byte = 0xC0; // id for uniaxial msg

    uniaxial_v = ADC_to_Voltage(uniaxial_adc);
    // uniaxial_hz = v to hz function

//    rs_msg.data[0] = 0;
//    rs_msg.data[1] = 0;
//    rs_msg.data[2] = (uint8_t)(uniaxial_v & 0xFF);
//    rs_msg.data[3] = (uint8_t)(uniaxial_v >> 8);
//    rs_msg.data[4] = (uint8_t)(uniaxial_hz & 0xFF);
//    rs_msg.data[5] = (uint8_t)(uniaxial_hz >> 8);

    rs_msg.data[0] = 0xC0;
    rs_msg.data[1] = 0xC0;
    rs_msg.data[2] = 0xC0;
    rs_msg.data[3] = 0xC0;
    rs_msg.data[4] = 0xC0;
    rs_msg.data[5] = 0xC0;

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
//    RS485_Enqueue(&rs_msg);
}

void RS485_Send(void){

    // if (Timer_HasElapsed(&RS485TimerTest)) {
    // 	Send_Test_RS485_Message();
	// }
	static EasyTimer_t RS485TimerA0 = {.start_time = 0, .interval = 30};  // 50ms
    if (Timer_HasElapsed(&RS485TimerA0)) {
    	Send_RS485_Message_A0();
//    	RS485_Process_Queue(&huart1);
//    	Send_RS485_Message_B0();
//    	Send_RS485_Message_C0();
	}

//    RS485_Process_Queue(&huart1);
//    static EasyTimer_t RS485TimerB0 = {.start_time = 0, .interval = 30};  // 50ms
//    if (Timer_HasElapsed(&RS485TimerB0)) {
//    	Send_RS485_Message_B0();
////    	RS485_Process_Queue(&huart1);
//	}
////
//    static EasyTimer_t RS485TimerC0 = {.start_time = 0, .interval = 30};  // 50ms
//    if (Timer_HasElapsed(&RS485TimerC0)) {
//    	Send_RS485_Message_C0();
////    	RS485_Process_Queue(&huart1);
//	}

}

#endif
