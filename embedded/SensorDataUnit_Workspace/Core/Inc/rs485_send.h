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

void Send_Combined_Message(void) {
    RS485_Message rs_msg;

    rs_msg.start_byte = 0xD0; // New unique ID for the "Combined" message

    triaxial_x_g = (ADXL372_GetG(triaxial_x_raw) * 100.0f);
    triaxial_y_g = (ADXL372_GetG(triaxial_y_raw) * 100.0f);
    triaxial_z_g = (ADXL372_GetG(triaxial_z_raw) * 100.0f);

    // 1. Pack Triaxial Data (6 bytes)
    rs_msg.data[0] = (uint8_t)(triaxial_x_g);
    rs_msg.data[1] = (uint8_t)(triaxial_x_g >> 8);
    rs_msg.data[2] = (uint8_t)(triaxial_y_g);
    rs_msg.data[3] = (uint8_t)(triaxial_y_g >> 8);
    rs_msg.data[4] = (uint8_t)(triaxial_z_g);
    rs_msg.data[5] = (uint8_t)(triaxial_z_g >> 8);

    // 2. Pack Strain Gauge (2 bytes)
    strain = (Get_Strain(ADC_to_Voltage(strain_gauge_adc)) * 100);
    rs_msg.data[6] = (uint8_t)(strain);
    rs_msg.data[7] = (uint8_t)(strain >> 8);

    // 3. Pack Uniaxial G-Force (4 bytes)
//    uniaxial_g = (int32_t)(Get_G_Force(ADC_to_Voltage(uniaxial_adc)) * 1000.0f);
    uniaxial_g = (Get_G_Force(ADC_to_Voltage(uniaxial_adc)) * 1000.0f);
    rs_msg.data[8] = (uint8_t)(uniaxial_g);
    rs_msg.data[9] = (uint8_t)(uniaxial_g >> 8);
    rs_msg.data[10] = (uint8_t)(uniaxial_g >> 16);
    rs_msg.data[11] = (uint8_t)(uniaxial_g >> 24);

    // 4. Bytes 12-15 are spare/padding for now
    for(int i=12; i<16; i++) rs_msg.data[i] = 0;

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
}

//void Send_Combined_Message(void) {
//    RS485_Message rs_msg;
//
//    rs_msg.start_byte = 0xD0; // New unique ID for the "Combined" message
//
////    triaxial_x_g = (int16_t)(ADXL372_GetG(triaxial_x_raw) * 100.0f);
////    triaxial_y_g = (int16_t)(ADXL372_GetG(triaxial_y_raw) * 100.0f);
////    triaxial_z_g = (int16_t)(ADXL372_GetG(triaxial_z_raw) * 100.0f);
//
//    // 1. Pack Triaxial Data (6 bytes)
//    rs_msg.data[0] = (uint8_t)(0x01);
//    rs_msg.data[1] = (uint8_t)(0x01);
//    rs_msg.data[2] = (uint8_t)(0x01);
//    rs_msg.data[3] = (uint8_t)(0x01);
//    rs_msg.data[4] = (uint8_t)(0x01);
//    rs_msg.data[5] = (uint8_t)(0x01);
//
//    // 2. Pack Strain Gauge (2 bytes)
////    uint16_t strain_v = ADC_to_Voltage(strain_gauge_adc);
//    rs_msg.data[6] = (uint16_t)(0x02);
//    rs_msg.data[7] = (uint16_t)(0x02);
//
//    // 3. Pack Uniaxial G-Force (4 bytes)
////    uniaxial_g = (int32_t)(Get_G_Force(ADC_to_Voltage(uniaxial_adc)) * 1000.0f);
//    rs_msg.data[8] = (uint8_t)(0x03);
//    rs_msg.data[9] = (uint8_t)(0x03);
//    rs_msg.data[10] = (uint8_t)(0x03);
//    rs_msg.data[11] = (uint8_t)(0x03);
//
//    // 4. Bytes 12-15 are spare/padding for now
////    for(int i=12; i<16; i++) rs_msg.data[i] = 0;
//
//    rs_msg.data[12] = (uint8_t)(0x00);
//    rs_msg.data[13] = (uint8_t)(0x00);
//    rs_msg.data[14] = (uint8_t)(0x00);
//    rs_msg.data[15] = (uint8_t)(0x00);
//
//    // Send it using UART1 handle
//    RS485_Write_Message(&rs_msg, &huart1);
//}

void Send_Message_A0(void) {
    RS485_Message rs_msg;

    // rs_msg.start_byte = 0xAA;
    rs_msg.start_byte = 0xA0; // id for triaxial x, y, and z msg

    // 16 bits per sig

//    triaxial_x_g = Raw_to_G(triaxial_x_raw);
//    triaxial_y_g = Raw_to_G(triaxial_y_raw);
//    triaxial_z_g = Raw_to_G(triaxial_z_raw);

    // send raw 12bit+ values here, decode on the receiver side (*0.1)

    rs_msg.data[0] = (int16_t)(triaxial_x_raw);
    rs_msg.data[1] = (int16_t)(triaxial_x_raw >> 8);
    rs_msg.data[2] = (int16_t)(triaxial_y_raw);
    rs_msg.data[3] = (int16_t)(triaxial_y_raw >> 8);
    rs_msg.data[4] = (int16_t)(triaxial_z_raw);
    rs_msg.data[5] = (int16_t)(triaxial_z_raw >> 8);

//    rs_msg.data[0] = 0xA0;
//	rs_msg.data[1] = 0xA0;
//	rs_msg.data[2] = 0xA0;
//	rs_msg.data[3] = 0xA0;
//	rs_msg.data[4] = 0xA0;
//	rs_msg.data[5] = 0xA0;

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
//    RS485_Enqueue(&rs_msg);
}

void Send_Message_B0(void) {
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
    rs_msg.data[4] = (uint8_t) (strain_gauge_diff_v);
    rs_msg.data[5] = (uint8_t) (strain_gauge_diff_v >> 8);

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
}

void Send_Message_C0(void) {
    RS485_Message rs_msg;

    // rs_msg.start_byte = 0xAA;
    rs_msg.start_byte = 0xC0; // id for uniaxial msg

//    uniaxial_v = ADC_to_Voltage(uniaxial_adc);
    uniaxial_v = (int32_t) Get_G_Force(ADC_to_Voltage(uniaxial_adc));
    // uniaxial_hz = v to hz function

//    rs_msg.data[0] = 0;
//    rs_msg.data[1] = 0;
//    rs_msg.data[2] = (uint8_t)(uniaxial_v & 0xFF);
//    rs_msg.data[3] = (uint8_t)(uniaxial_v >> 8);
//    rs_msg.data[4] = (uint8_t)(uniaxial_hz & 0xFF);
//    rs_msg.data[5] = (uint8_t)(uniaxial_hz >> 8);

    rs_msg.data[0] = 0xC0;
    rs_msg.data[1] = 0xC0;
    rs_msg.data[2] = (int32_t) (uniaxial_v);
    rs_msg.data[3] = (int32_t) (uniaxial_v >> 8);
    rs_msg.data[4] = (int32_t) (uniaxial_v >> 16);
    rs_msg.data[5] = (int32_t) (uniaxial_v >> 24);

    // Send it using UART1 handle
    RS485_Write_Message(&rs_msg, &huart1);
}

void RS485_Send(void){

    // if (Timer_HasElapsed(&RS485TimerTest)) {
    // 	Send_Test_RS485_Message();
	// }

	// 50ms currently, add clock to improve baud rate and decrease interval time
//	static EasyTimer_t RS485TimerA0 = {.start_time = 0, .interval = 50};
//    if (Timer_HasElapsed(&RS485TimerA0)) {
//    	Send_Message_A0();
//	}
//
//    static EasyTimer_t RS485TimerB0 = {.start_time = 0, .interval = 50};
//    if (Timer_HasElapsed(&RS485TimerB0)) {
//        Send_Message_B0();
//    }
//
//    static EasyTimer_t RS485TimerC0 = {.start_time = 0, .interval = 50};
//	if (Timer_HasElapsed(&RS485TimerC0)) {
//		Send_Message_C0();
//	}

	static EasyTimer_t RS485TimerA0 = {.start_time = 0, .interval = 21};
	if (Timer_HasElapsed(&RS485TimerA0)) {
		Send_Combined_Message();
	}

}

#endif
