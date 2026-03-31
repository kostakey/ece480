#ifndef __TCAN_
#define __TCAN_

#include "main.h"

SPI_HandleTypeDef hspi3;

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//   hspi3.Init.CRCPolynomial = 7;
//   hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
//   hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

//void TCAN4550_WriteReg(uint16_t address, uint32_t data) {
//    uint8_t spi_buf[8];
//
//    // 1. Header (0x61 is Write)
//    spi_buf[0] = 0x61;
//
//    // 2. Address - The TCAN expects the 16-bit address here
//    spi_buf[1] = (uint8_t)(address >> 8);
//    spi_buf[2] = (uint8_t)(address & 0xFF);
//
//    // 3. Length - Number of WORDS (1 word = 4 bytes)
//    spi_buf[3] = 0x01;
//
//    // 4. Data - Big Endian
//    spi_buf[4] = (uint8_t)((data >> 24) & 0xFF);
//    spi_buf[5] = (uint8_t)((data >> 16) & 0xFF);
//    spi_buf[6] = (uint8_t)((data >> 8) & 0xFF);
//    spi_buf[7] = (uint8_t)(data & 0xFF);
//
//    // 5. Transfer with a small delay before/after CS to ensure timing
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_RESET);
//    // Add a tiny NOP or delay if your SPI is very fast (>10MHz)
//    HAL_SPI_Transmit(&hspi3, spi_buf, 8, 100);
//    // Ensure the last bit has shifted out before pulling CS high
//    while(HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY);
//    for(volatile int i=0; i<100; i++);
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_SET);
//}

void TCAN4550_WriteReg(uint16_t address, uint32_t data) {
    uint8_t spi_buf[8];

    spi_buf[0] = 0x61;             // Write Opcode
    spi_buf[1] = (uint8_t)(address >> 8);
    spi_buf[2] = (uint8_t)(address & 0xFF);
    spi_buf[3] = 0x01;             // 1 word

    spi_buf[4] = (uint8_t)(data >> 24);
    spi_buf[5] = (uint8_t)(data >> 16);
    spi_buf[6] = (uint8_t)(data >> 8);
    spi_buf[7] = (uint8_t)(data & 0xFF);

    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_RESET);

    // 1. Transmit the 8 bytes
    HAL_SPI_Transmit(&hspi3, spi_buf, 8, 100);

    // 2. CRITICAL: Wait for the STM32 SPI peripheral to be completely IDLE
    // Without this, CS might go high while the 64th bit is still on the wire.
    while(__HAL_SPI_GET_FLAG(&hspi3, SPI_FLAG_BSY));

    // 3. Tiny delay (approx 1us) to ensure the TCAN logic captures the edge
    for(volatile int i=0; i<500; i++);

    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_SET);
}

//void TCAN4550_WriteReg(uint16_t address, uint32_t data) {
//    uint8_t spi_buf[8];
//
//    spi_buf[0] = 0x61;             // Write
//    spi_buf[1] = (address >> 8);   // Addr H
//    spi_buf[2] = (address & 0xFF); // Addr L
//    spi_buf[3] = 0x01;             // 1 Word
//
//    spi_buf[4] = (data >> 24);     // Data MSB
//    spi_buf[5] = (data >> 16);
//    spi_buf[6] = (data >> 8);
//    spi_buf[7] = (data & 0xFF);    // Data LSB
//
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_RESET);
//
//    // Tiny setup delay
//    for(volatile int i=0; i<50; i++);
//
//    HAL_SPI_Transmit(&hspi3, spi_buf, 8, 100);
//
//    // CRITICAL FIX: Ensure the SPI BUS is actually idle
//    // before we yank the CS line high.
//    while(__HAL_SPI_GET_FLAG(&hspi3, SPI_FLAG_BSY));
//
//    // Tiny hold delay to ensure the 64th bit is latched
//    for(volatile int i=0; i<50; i++);
//
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_SET);
//}

//// Example: Setting the TCAN4550 to 'Normal' Mode
//void TCAN4550_WriteReg(uint16_t address, uint32_t data) {
//    uint8_t spi_buf[8]; // was 8, may be issues there
//
//    // 1. Header & Address
//    spi_buf[0] = 0x61;             // Write Opcode
//    spi_buf[1] = (address >> 8);   // MSB of Address
//    spi_buf[2] = (address & 0xFF); // LSB of Address
//
//    // 2. Length (Number of 32-bit words)
//    spi_buf[3] = 0x01;             // Writing 1 word (4 bytes)
//
//    // 3. Data (TCAN4550 expects Big Endian for data words)
//    spi_buf[4] = (data >> 24) & 0xFF;
//    spi_buf[5] = (data >> 16) & 0xFF;
//    spi_buf[6] = (data >> 8) & 0xFF;
//    spi_buf[7] = (data & 0xFF);
//
//    // 4. Perform SPI Transfer
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_RESET); // Pull CS Low
//    HAL_SPI_Transmit(&hspi3, spi_buf, 8, 100);        // Send the 8-byte packet
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_SET);   // Pull CS High
//}

//uint32_t TCAN4550_ReadReg(uint16_t address) {
//    uint8_t tx_buf[4];
//    uint8_t rx_buf[4];
//    uint32_t reg_value = 0;
//
//    // 1. Prepare Header: Read Opcode (0x41) + 16-bit Address + Length (1 word)
//    tx_buf[0] = 0x41;               // Read Opcode
//    tx_buf[1] = (address >> 8) & 0xFF;     // Address MSB
//    tx_buf[2] = (address & 0xFF);   // Address LSB
//    tx_buf[3] = 0x01;               // Number of 32-bit words to read
//
//    // 2. Start SPI Transaction
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_RESET); // Replace with your nCS Pin
//
//    // 3. Send the Header
//    HAL_SPI_Transmit(&hspi3, tx_buf, 4, 100);
//
//    // 4. Receive the 4 bytes of data
//    HAL_SPI_Receive(&hspi3, rx_buf, 4, 100);
//
//    // 5. End SPI Transaction
//    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_SET);
//
//    // 6. Combine bytes (TCAN4550 returns data in Big Endian)
//    reg_value = ((uint32_t)rx_buf[0] << 24) |
//                ((uint32_t)rx_buf[1] << 16) |
//                ((uint32_t)rx_buf[2] << 8)  |
//                ((uint32_t)rx_buf[3]);
//
//    return reg_value;
//}

uint32_t TCAN4550_ReadReg(uint16_t address) {
    uint8_t tx_buf[4];
    uint8_t rx_buf[4];

    tx_buf[0] = 0x41; // Read Opcode
    tx_buf[1] = (uint8_t)(address >> 8);
    tx_buf[2] = (uint8_t)(address & 0xFF);
    tx_buf[3] = 0x01; // 1 word

    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, tx_buf, 4, 10);
    // Some TCAN chips need a tiny pause between header and data
    HAL_SPI_Receive(&hspi3, rx_buf, 4, 10);
    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_SET);

    // Reassemble: TCAN sends MSB first
    return ((uint32_t)rx_buf[0] << 24) |
           ((uint32_t)rx_buf[1] << 16) |
           ((uint32_t)rx_buf[2] << 8)  |
           ((uint32_t)rx_buf[3]);
}

//void TCAN_Write_Timing_Manual(void) {
//    uint8_t tx[8];
//    tx[0] = 0x61; // OpCode
//    tx[1] = 0x10; // Addr H
//    tx[2] = 0x1C; // Addr L
//    tx[3] = 0x01; // Length (1 word)
//
//    // For 500k @ 40MHz, we want 0x000F3F0F
//    // We must send MSB first (Bits 31-24)
//    tx[4] = 0x00; // Bits 31-24
//    tx[5] = 0x0F; // Bits 23-16
//    tx[6] = 0x3F; // Bits 15-8
//    tx[7] = 0x0F; // Bits 7-0
//
//    HAL_GPIO_WritePin(CS_PORT, CS_PIN, 0);
//    HAL_SPI_Transmit(&hspi3, tx, 8, 100);
//    HAL_GPIO_WritePin(CS_PORT, CS_PIN, 1);
//}


void TCAN4550_Enable_Normal_Mode(void) {
    // 1. Clear all Interrupts (Writing a 1 to a bit clears it)
    // Register 0x0820 is the Main Interrupt register
    TCAN4550_WriteReg(0x0820, 0xFFFFFFFF);

    // 2. Set Mode to Normal
    // Register 0x0800: Bits 7:6 are Mode Selection
    // 10 = Normal, 01 = Standby
    TCAN4550_WriteReg(0x0800, 0x84000000);

    // 3. Verify the change
    uint32_t mode_check = TCAN4550_ReadReg(0x0800);

    // Check if bits [7:6] are '10' (Normal)
    if ((mode_check & 0xC0000000) == 0x80000000) {
        // SUCCESS: The transceiver is now active!
    }
}

//void TCAN4550_Configure_CAN(void) {
//    // 1. Enter Initialization Mode (Set INIT and CCE bits)
//    TCAN4550_WriteReg(0x1018, 0x00000003);
//
//    // Wait for acknowledgment
//    while (!(TCAN4550_ReadReg(0x1018) & 0x00000001));
//
//    // 2. Set TX Buffer Configuration (Still needed for Classic CAN)
//    // Tells the chip TX buffers start at 0x8000 in RAM
//    TCAN4550_WriteReg(0x10C8, 0x01000000);
//
//    // 3. Set Nominal Bit Timing (500 kbps @ 40MHz Clock)
//    // NBTP (0x101C): NSJW=15, NTSEG1=63, NTSEG2=15, NBRP=0 (Prescaler 1)
//    // Formula: BitRate = Clock / (Prescaler * (Sync + TSEG1 + TSEG2))
//    // 40MHz / (1 * (1 + 64 + 15)) = 500kbps
//    TCAN4550_WriteReg(0x101C, 0x000F3F0F);
//
//    // 4. DISABLE FD and BRS (Classic CAN 2.0 Mode)
//    // We clear bit 9 (FDOE) and bit 8 (BRSE) in CCCR
//    uint32_t cccr = TCAN4550_ReadReg(0x1018);
//    cccr &= ~((1 << 9) | (1 << 8));
//    TCAN4550_WriteReg(0x1018, cccr);
//
//    // 5. Exit Initialization Mode
//    // Clear INIT and CCE to start the CAN engine
//    cccr &= ~(0x00000003);
//    TCAN4550_WriteReg(0x1018, cccr);
//
//    HAL_Delay(10);
//}

void TCAN4550_Configure_CAN(void) {
    // 1. Enter Init Mode first (to change settings)
    TCAN4550_WriteReg(0x1018, 0x00000003); 
    while (!(TCAN4550_ReadReg(0x1018) & 0x00000001));

    // 2. IMPORTANT: Set the Clock Divider
    // If you are using 40MHz, make sure the divider is set to 1
    // Register 0x0800: Bits 15:10 are the clock divider
    // Let's ensure bit 9 (WAKEUP) isn't causing a reset loop
    TCAN4550_WriteReg(0x0800, 0x84000008);

    // 3. Clear the Configuration Change Enable and Init bits
    // We must write 0x0 to clear bits 0 and 1.
    // If you want CAN 2.0 (No FD), make sure FD bits are also 0.
    TCAN4550_WriteReg(0x1018, 0x00000003);

    // 4. CHECK IF IT WORKED
    // If check2 is still 0x19 here, the hardware is overriding your SPI write.
}

void TCAN4550_Transmit_Test_Frame(void) {
    // Word 0: Standard ID 0x123 (Shifted left by 18)
//    TCAN4550_WriteReg(0x8000, (0x123 << 18));
//
//    // Word 1: DLC and Mode
//    // Bit 21 (ESI), Bit 15 (EDL), Bit 14 (BRS) must be 0 for Classic CAN
//    // DLC = 8 bytes (0x8 in bits 19:16)
//    TCAN4550_WriteReg(0x8004, 0x00080000);
//
//    // Word 2 & 3: Data (8 bytes total)
//    TCAN4550_WriteReg(0x8008, 0x11223344);
//    TCAN4550_WriteReg(0x800C, 0x55667788);
//
//    // Trigger Transmission (Buffer 0)
//    TCAN4550_WriteReg(0x10D0, 0x00000001);

    // 1. Prepare the Header (Word 0 & 1)
	// Word 0: Standard ID 0x123. ID starts at bit 18 for 11-bit IDs.
//	TCAN4550_WriteReg(0x8000, (0x123 << 18));
//
//	// Word 1: Control bits + DLC
//	// Bits 19:16 is DLC (8 bytes = 0x8).
//	// We set bit 30 (Message Marker) to 0xAA just for tracking.
//	TCAN4550_WriteReg(0x8004, 0x00080000);
//
//	// 2. Prepare the Data (Word 2 & 3)
//	TCAN4550_WriteReg(0x8088, 0xDEADC0DE); // Bytes 0-3
//	TCAN4550_WriteReg(0x808C, 0x0000BEEF); // Bytes 4-7
//
//	// 3. Command the Transmission
//	// Set Bit 0 of TXBAR (0x10D0) to request Buffer 0
//	TCAN4550_WriteReg(0x10D0, 0x00000001);
	// Ensure the Request is clear before starting
	TCAN4550_WriteReg(0x10D8, 0x00000001); // Cancel any old Buffer 0 request

	// Word 0: ID 0x123 (Standard)
	TCAN4550_WriteReg(0x8080, (0x123 << 18));

	// Word 1: DLC = 8, and set 'Message Marker' (Bit 24-31) to 0x01
	// Setting a marker helps the core track the event.
	TCAN4550_WriteReg(0x8084, 0x01080000);

	// Word 2 & 3: Data
	TCAN4550_WriteReg(0x8088, 0x11223344);
	TCAN4550_WriteReg(0x808C, 0x55667788);

	// TRIGGER
	TCAN4550_WriteReg(0x10D0, 0x00000001);

}

void TCAN4550_Force_Init_Manual(void) {
    uint8_t txData[8];

    // Command: Write to 0x1018 (CCCR)
    txData[0] = 0x61; // Write
    txData[1] = 0x10; // Addr High
    txData[2] = 0x18; // Addr Low
    txData[3] = 0x01; // 1 Word

    // We want to write 0x00000003 (INIT + CCE)
    // Because your SPI swaps words, we send 0x00030000
    txData[4] = 0x00;
    txData[5] = 0x03; // This lands in the second byte (Bits 16-23)
    txData[6] = 0x00;
    txData[7] = 0x00;

    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi3, txData, 8, 100);
    HAL_GPIO_WritePin(GPIOA, CAN1_SPI_CS_Pin, GPIO_PIN_SET);
}

#endif
