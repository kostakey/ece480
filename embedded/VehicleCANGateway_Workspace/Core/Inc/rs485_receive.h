#ifndef __RECEIVERS485_
#define __RECEIVERS485_

#include "rs485.h"
#include "sensors.h"

void RS485bus_Read(void){
//  RS485_Message rx_buffer;

//  RS485_Read_Message(&huart1, &rx_buffer);

  static uint8_t my_local_rx[8];
  static uint8_t index = 0;
  uint8_t rx_temp;

  HAL_StatusTypeDef status = HAL_UART_Receive(&huart1, &rx_temp, 1, 0);

  if (status == HAL_OK) {
	  if (index == 0) {
		  // Filter for any valid starting ID
		  if (rx_temp == 0xA0 || rx_temp == 0xB0 || rx_temp == 0xC0) {
			  my_local_rx[index] = rx_temp;
			  index++;
		  }
	  } else {
		  my_local_rx[index] = rx_temp;
		  index++;

		  if (index >= 8) {
			  // Process based on which ID we caught at the start
			  if (my_local_rx[0] == 0xA0) {
				  // Store in triaxial variables
//				  watchpoint_90 = my_local_rx[0];
//				  watchpoint_91 = my_local_rx[1];
//				  watchpoint_92 = my_local_rx[2];
//				  watchpoint_93 = my_local_rx[3];
//				  watchpoint_94 = my_local_rx[4];
//				  watchpoint_95 = my_local_rx[5];
//				  watchpoint_96 = my_local_rx[6];
//				  watchpoint_97 = my_local_rx[7];

				  triaxial_x_g = (my_local_rx[1] << 8) | my_local_rx[0];
				  triaxial_y_g = (my_local_rx[3] << 8) | my_local_rx[2];
				  triaxial_z_g = (my_local_rx[5] << 8) | my_local_rx[4];

			  } else if (my_local_rx[0] == 0xB0) {
				  // Store in strain gauge variables
//				  watchpoint_80 = my_local_rx[0];
//				  watchpoint_81 = my_local_rx[1];
//				  watchpoint_82 = my_local_rx[2];
//				  watchpoint_83 = my_local_rx[3];
//				  watchpoint_84 = my_local_rx[4];
//				  watchpoint_85 = my_local_rx[5];
//				  watchpoint_86 = my_local_rx[6];
//				  watchpoint_87 = my_local_rx[7];
				  strain_gauge_diff_v = (my_local_rx[3] << 8) | my_local_rx[2];
				  strain_gauge_hz     = (my_local_rx[5] << 8) | my_local_rx[4];

			  } else if (my_local_rx[0] == 0xC0) {
				  // Store in strain gauge variables
//				  watchpoint_80 = my_local_rx[0];
//				  watchpoint_81 = my_local_rx[1];
//				  watchpoint_82 = my_local_rx[2];
//				  watchpoint_83 = my_local_rx[3];
//				  watchpoint_84 = my_local_rx[4];
//				  watchpoint_85 = my_local_rx[5];
//				  watchpoint_86 = my_local_rx[6];
//				  watchpoint_87 = my_local_rx[7];
				  uniaxial_v  = (my_local_rx[3] << 8) | my_local_rx[2];
				  uniaxial_hz = (my_local_rx[5] << 8) | my_local_rx[4];
			  }

			  index = 0; // Reset
		  }
	  }
}


#endif
