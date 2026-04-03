#ifndef __TCAN_
#define __TCAN_

#include "main.h"

extern SPI_HandleTypeDef hspi3;

typedef struct {
    uint32_t id;      // CAN ID
    uint8_t  len;     // DLC (0-8)
    uint8_t  data[8]; // Payload
} TCAN_Message;

void TCAN4550_WriteReg(uint16_t address, uint32_t data, uint16_t chip_select) {
	uint8_t spi_buf[8];

	spi_buf[0] = 0x61;             // Write Opcode
	spi_buf[1] = (uint8_t)(address >> 8);
	spi_buf[2] = (uint8_t)(address & 0xFF);
	spi_buf[3] = 0x01;             // 1 word

	spi_buf[4] = (uint8_t)(data >> 24);
	spi_buf[5] = (uint8_t)(data >> 16);
	spi_buf[6] = (uint8_t)(data >> 8);
	spi_buf[7] = (uint8_t)(data & 0xFF);

	HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);

	// 1. Transmit the 8 bytes
	HAL_SPI_Transmit(&hspi3, spi_buf, 8, 100);

	// 2. CRITICAL: Wait for the STM32 SPI peripheral to be completely IDLE
	// Without this, CS might go high while the 64th bit is still on the wire.
	//    while(__HAL_SPI_GET_FLAG(&hspi3, SPI_FLAG_BSY));

	// 3. Tiny delay (approx 1us) to ensure the TCAN logic captures the edge
	//    for(volatile int i=0; i<500; i++);

	HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);
}

uint32_t TCAN4550_ReadReg(uint16_t address, uint16_t chip_select) {
    uint8_t tx_buf[4];
    uint8_t rx_buf[4];

    tx_buf[0] = 0x41; // Read Opcode
    tx_buf[1] = (uint8_t)(address >> 8);
    tx_buf[2] = (uint8_t)(address & 0xFF);
    tx_buf[3] = 0x01; // 1 word

    //kinda hard coded for port number

    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, tx_buf, 4, 10);
    // Some TCAN chips need a tiny pause between header and data
    HAL_SPI_Receive(&hspi3, rx_buf, 4, 10);
    HAL_GPIO_WritePin(GPIOA, chip_select, GPIO_PIN_SET);

    // Reassemble: TCAN sends MSB first
    return ((uint32_t)rx_buf[0] << 24) |
           ((uint32_t)rx_buf[1] << 16) |
           ((uint32_t)rx_buf[2] << 8)  |
           ((uint32_t)rx_buf[3]);
}

void TCAN4550_Init(uint16_t chip_select) {

	TCAN4550_WriteReg(0x0800, 0xC8000084, chip_select);
	HAL_Delay(10); // Wait for reset to toggle
	// Disable Watchdog (Bit 3 = 0) and set to Normal Mode (Bits 7:6 = 10)
	TCAN4550_WriteReg(0x0800, 0xC8000080, chip_select); // watchdog times out after 60ms by default, so make sure to config on startup
	HAL_Delay(100);

	//  TCAN4550_WriteReg(0x0820, 0x00100000);
	TCAN4550_WriteReg(0x0820, 0xFFFFFFFF, chip_select);
	HAL_Delay(5);

    //	check0820 = TCAN4550_ReadReg(0x0820);

	TCAN4550_WriteReg(0x0824, 0x00000000, chip_select);
	//  TCAN4550_WriteReg(0x0824, 0xFFFFFFFF);
	HAL_Delay(5);

    //	check0824 = TCAN4550_ReadReg(0x0824);

	//  TCAN4550_WriteReg(0x000C, 0x00000000);
	TCAN4550_WriteReg(0x000C, 0xFFFFFFFF, chip_select);
	HAL_Delay(5);

	TCAN4550_WriteReg(0x1018, 0x00000001, chip_select);

	TCAN4550_WriteReg(0x1018, 0x00000003, chip_select); // init
    //	check1018 = TCAN4550_ReadReg(0x1018);

	for (uint32_t addr = 0x8000; addr < 0x8300; addr += 4) {
	    TCAN4550_WriteReg(addr, 0x00000000, chip_select);
	} // clear mram before reallocating

	TCAN4550_WriteReg(0x101C, 0x00011F07, chip_select);

	TCAN4550_WriteReg(0x100C, 0x00000701, chip_select);

	// TX Buffer Configuration: 1 element, starts at offset 0x0270
	// (Matches 0x8270 in memory)
	TCAN4550_WriteReg(0x10C0, 0x01000270, chip_select);
    //	check10C0 = TCAN4550_ReadReg(0x10C0);
	// TX Element Size: 64 Bytes (0x7)
	TCAN4550_WriteReg(0x10C8, 0x00000007, chip_select);

	// RX FIFO 0 Configuration: 4 elements, starts at offset 0x0010
	TCAN4550_WriteReg(0x10A0, 0x02040010, chip_select);
    //	check10A0 = TCAN4550_ReadReg(0x10A0);
	// RX Element Size: FIFO0 = 64 Bytes (0x7)
	TCAN4550_WriteReg(0x10BC, 0x00000077, chip_select);

	TCAN4550_WriteReg(0x1084, 0x00000000, chip_select); // SIDFC
	TCAN4550_WriteReg(0x1088, 0x00000000, chip_select); // XIDFC

	TCAN4550_WriteReg(0x1018, 0x00000000, chip_select); // exit init

}

void TCAN4550_Send_Test_Message(uint16_t chip_select) {

	TCAN4550_WriteReg(0x1018, 0x00000003, chip_select); // init
	TCAN4550_WriteReg(0x8270, 0x448C0000, chip_select); // ID 0x123
//	check8270 = TCAN4550_ReadReg(0x8270);
	TCAN4550_WriteReg(0x8274, 0x00080000, chip_select); // DLC 8
	TCAN4550_WriteReg(0x8278, 0x4C4C4548, chip_select); // "HELL"
	TCAN4550_WriteReg(0x827C, 0x2121214F, chip_select); // "O!!!"
	TCAN4550_WriteReg(0x1018, 0x00000000, chip_select); // exit init

	TCAN4550_WriteReg(0x10D0, 0x00000001, chip_select); // send it

}

void TCAN4550_Write_Message(TCAN_Message *msg, uint16_t chip_select) {
    uint32_t headerW0 = 0;
    uint32_t headerW1 = 0;

    // 1. Prepare Word 0: Standard ID
    // Shift ID 18 bits left (for Standard ID) and ensure XTD (bit 30) is 0
    headerW0 = (msg->id & 0x7FF) << 18;
    TCAN4550_WriteReg(0x8270, headerW0, chip_select);

    // 2. Prepare Word 1: DLC
    // Bits 19:16 are DLC. Ensure FDF (bit 21) is 0 for CAN 2.0
    headerW1 = (uint32_t)(msg->len & 0x0F) << 16;
    TCAN4550_WriteReg(0x8274, headerW1, chip_select);

    // 3. Prepare Data Bytes (Big Endian pack for "HELLO!!!")
    uint32_t dataW2 = (msg->data[3] << 24) | (msg->data[2] << 16) | (msg->data[1] << 8) | msg->data[0];
    uint32_t dataW3 = (msg->data[7] << 24) | (msg->data[6] << 16) | (msg->data[5] << 8) | msg->data[4];

    TCAN4550_WriteReg(0x8278, dataW2, chip_select);
    TCAN4550_WriteReg(0x827C, dataW3, chip_select);

    // 4. Request Transmission (Add to TX Request Pending)
    TCAN4550_WriteReg(0x10D0, 0x00000001, chip_select);
}

#endif
