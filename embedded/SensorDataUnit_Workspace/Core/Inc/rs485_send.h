#ifndef __SENDRS485_
#define __SENDRS485_

#include "rs485.h"
#include "easytimer.h"
#include "sensors.h"
#include "adc.h"

/*
Processes raw sensor data into physical units and packages them into a single RS485 message with ID 0xD0.
*/
void Send_Combined_Message(void) {
    RS485_Message rs_msg;

    rs_msg.start_byte = 0xD0; // New unique ID for the "Combined" message

    triaxial_x_g = (ADXL372_GetG(triaxial_x_raw) * 100.0f);
    triaxial_y_g = (ADXL372_GetG(triaxial_y_raw) * 100.0f);
    triaxial_z_g = (ADXL372_GetG(triaxial_z_raw) * 100.0f);

    rs_msg.data[0] = (uint8_t)(triaxial_x_g);
    rs_msg.data[1] = (uint8_t)(triaxial_x_g >> 8);
    rs_msg.data[2] = (uint8_t)(triaxial_y_g);
    rs_msg.data[3] = (uint8_t)(triaxial_y_g >> 8);
    rs_msg.data[4] = (uint8_t)(triaxial_z_g);
    rs_msg.data[5] = (uint8_t)(triaxial_z_g >> 8);

    strain = (Get_Strain(ADC_to_Voltage(strain_gauge_adc)) * 100);
    rs_msg.data[6] = (uint8_t)(strain);
    rs_msg.data[7] = (uint8_t)(strain >> 8);

    uniaxial_g = (Get_G_Force(ADC_to_Voltage(uniaxial_adc)) * 1000.0f);
    rs_msg.data[8] = (uint8_t)(uniaxial_g);
    rs_msg.data[9] = (uint8_t)(uniaxial_g >> 8);
    rs_msg.data[10] = (uint8_t)(uniaxial_g >> 16);
    rs_msg.data[11] = (uint8_t)(uniaxial_g >> 24);

    for(int i=12; i<16; i++) rs_msg.data[i] = 0;

    RS485_Write_Message(&rs_msg, &huart1);
}

/*
Packages raw triaxial accelerometer data (X, Y, Z) into
an RS485 message with ID 0xA0.
*/
void Send_Message_A0(void) {
    RS485_Message rs_msg;

    // rs_msg.start_byte = 0xAA;
    rs_msg.start_byte = 0xA0; // id for triaxial x, y, and z msg

    // 16 bits per sig
    rs_msg.data[0] = (int16_t)(triaxial_x_raw);
    rs_msg.data[1] = (int16_t)(triaxial_x_raw >> 8);
    rs_msg.data[2] = (int16_t)(triaxial_y_raw);
    rs_msg.data[3] = (int16_t)(triaxial_y_raw >> 8);
    rs_msg.data[4] = (int16_t)(triaxial_z_raw);
    rs_msg.data[5] = (int16_t)(triaxial_z_raw >> 8);

    RS485_Write_Message(&rs_msg, &huart1);
}

/*
Converts strain gauge ADC values to voltage and packages the result
into an RS485 message with ID 0xB0.
*/
void Send_Message_B0(void) {
    RS485_Message rs_msg;

    rs_msg.start_byte = 0xB0; // id for strain gauge diff and freq msg

    strain_gauge_diff_v = ADC_to_Voltage(strain_gauge_adc);
    
    rs_msg.data[0] = 0xB0;
    rs_msg.data[1] = 0xB0;
    rs_msg.data[2] = 0xB0;
    rs_msg.data[3] = 0xB0;
    rs_msg.data[4] = (uint8_t) (strain_gauge_diff_v);
    rs_msg.data[5] = (uint8_t) (strain_gauge_diff_v >> 8);

    RS485_Write_Message(&rs_msg, &huart1);
}

/*
Calculates G-force from the uniaxial sensor and packages the 32-bit
result into an RS485 message with ID 0xC0.
*/
void Send_Message_C0(void) {
    RS485_Message rs_msg;

    rs_msg.start_byte = 0xC0; // id for uniaxial msg

    uniaxial_v = (int32_t) Get_G_Force(ADC_to_Voltage(uniaxial_adc));

    rs_msg.data[0] = 0xC0;
    rs_msg.data[1] = 0xC0;
    rs_msg.data[2] = (int32_t) (uniaxial_v);
    rs_msg.data[3] = (int32_t) (uniaxial_v >> 8);
    rs_msg.data[4] = (int32_t) (uniaxial_v >> 16);
    rs_msg.data[5] = (int32_t) (uniaxial_v >> 24);

    RS485_Write_Message(&rs_msg, &huart1);
}

/*
Main transmission handler that uses a non-blocking timer to
periodically trigger the combined data message.
*/
void RS485_Send(void){

	static EasyTimer_t RS485TimerA0 = {.start_time = 0, .interval = 21};
	if (Timer_HasElapsed(&RS485TimerA0)) {
		Send_Combined_Message();
	}

}

#endif
