/*
 * CAN_controller_driver.c
 *
 * Created: 13.10.2016 09:07:30
 *  Author: Whiskey Dicks
 */

#include "CAN_controller_driver.h"
#include "bit_macros.h"

#include <avr/io.h>
#include <avr/delay.h>

void mcp_activate_slave(){
    /* Activate Slave Select */
    clear_bit(PORTB, PB1);
}

void mcp_deactivate_slave(){
    /* Deactivate Slave Select */
    set_bit(PORTB, PB1);
}

void mcp_2515_set_mode(uint8_t mode){
    mcp_2515_write(MCP_CANCTRL, mode);
}
/*
uint8_t mcp_2515_init(uint8_t mode){
    set_bit(DDRB , PB1); // Can Slave Select as output
	
    uint8_t val;
    SPI_init();
    
    mcp_2515_reset();
    _delay_ms(10);
    //printf("CANSTAT after reset: 0x%02X\n", mcp_2515_read(MCP_CANSTAT));
    mcp_2515_set_mode(MODE_CONFIG);

    val = mcp_2515_read(MCP_CANSTAT);
    uint8_t mode_bits = (val & MODE_MASK);
    if(mode_bits != MODE_CONFIG){
        printf("MCP2515 is NOT in Configuration mode after reset! Its config bits are %x\n", mode_bits);
        return 1;
    }

    mcp_2515_set_mode(mode);

    val = mcp_2515_read(MCP_CANSTAT);
    mode_bits = (val & MODE_MASK);

    if(mode_bits != mode){
        printf("MCP2515 is NOT in correct mode after reset! Its config bits are %x\n", mode_bits);
        return 1;
    }

    return 0;
}*/
 /*
uint8_t mcp_2515_init(uint8_t mode) {
    set_bit(DDRB, PB1); // CAN CS pin as output
	
    uint8_t val;
    SPI_init();
    
    // Reset MCP2515
    mcp_2515_reset();
    _delay_ms(10);

    // Enter Configuration mode
    mcp_2515_set_mode(MODE_CONFIG);

    val = mcp_2515_read(MCP_CANSTAT);
    uint8_t mode_bits = (val & MODE_MASK);
    if (mode_bits != MODE_CONFIG) {
        printf("MCP2515 is NOT in Configuration mode after reset! Its config bits are 0x%02X\n", mode_bits);
        return 1;
    }

    // ---------------------------------------------------------------------
    // Set bit timing registers for 500 kbps @ 16 MHz
    //
    // Formula: Bitrate = Fosc / (2 × (BRP + 1) × (1 + PROPSEG + PHSEG1 + PHSEG2))
    // Configuration below: BRP=1, PROPSEG=2, PHSEG1=5, PHSEG2=5, SJW=1
    // Results in ? 500 kbps
    // ---------------------------------------------------------------------

    mcp_2515_write(MCP_CNF1, 0x00 | (1 << 6));   // SJW = 1 TQ, BRP = 1 ? CNF1 = 0x01 (SJW=1, BRP=1)
    mcp_2515_write(MCP_CNF2, 0x90 | 0x05);       // BTLMODE=1, PHSEG1=5, PROPSEG=2 ? CNF2 = 0x90 + (PHSEG1)
    mcp_2515_write(MCP_CNF3, 0x02 | 0x05);       // SOF=0, PHSEG2=5 ? CNF3 = 0x05

    // Optional: Enable interrupts
    mcp_2515_write(MCP_CANINTE, 0x03); // Enable RX0IE and RX1IE

    // Clear error flags and buffers
    mcp_2515_bit_modify(MCP_EFLG, 0xFF, 0x00);
    mcp_2515_bit_modify(MCP_TXB0CTRL, 0x08, 0x00);

    // Finally, set requested mode (e.g. NORMAL)
    mcp_2515_set_mode(mode);

    val = mcp_2515_read(MCP_CANSTAT);
    mode_bits = (val & MODE_MASK);

    if (mode_bits != mode) {
        printf("MCP2515 is NOT in correct mode after configuration! Its config bits are 0x%02X\n", mode_bits);
        return 1;
    }

    printf("MCP2515 initialized at 500 kbps.\n");
    return 0;
}
*/
uint8_t mcp_2515_init(uint8_t mode) {
    set_bit(DDRB, PB1); // CAN CS pin as output

    uint8_t val;
    SPI_init();

    // Reset MCP2515
    mcp_2515_reset();
    _delay_ms(10);

    // Enter Configuration mode
    mcp_2515_set_mode(MODE_CONFIG);

    val = mcp_2515_read(MCP_CANSTAT);
    uint8_t mode_bits = (val & MODE_MASK);
    if (mode_bits != MODE_CONFIG) {
        printf("MCP2515 is NOT in Configuration mode after reset! Mode bits = 0x%02X\n", mode_bits);
        return 1;
    }

    // ==========================================================
    // Bit timing configuration for 500 kbps @ 16 MHz
    //
    // Formula:  Bitrate = Fosc / (2 * (BRP + 1) * (1 + PROPSEG + PHSEG1 + PHSEG2))
    // Choose:    BRP = 1  ? TQ = 2*(1+1)/16MHz = 250ns
    //            PROPSEG = 2, PHSEG1 = 3, PHSEG2 = 3 ? 9 TQ total
    // Bit Time = 9 * 250ns = 2.25µs ? 1 / 2.25µs ? 444 kbps
    // Adjusted to 500 kbps ? use 16 TQ total with BRP=1 (2 MHz per bit)
    // CNF1: SJW=1, BRP=1 (0x01)
    // CNF2: BTLMODE=1, PHSEG1=3, PROPSEG=2 (0x90 | 0x03 | (2 << 3) = 0x93)
    // CNF3: PHSEG2=3 (0x02)
    // ==========================================================

    // 500 kbps @ 16 MHz (proven good)
    //mcp_2515_write(MCP_CNF1, 0x00);  // SJW = 1, BRP = 0
    //mcp_2515_write(MCP_CNF2, 0x90 | (3 << 3) | 2); // BTLMODE=1, PHSEG1=3, PROPSEG=2 => 0x9A
    //mcp_2515_write(MCP_CNF3, 0x02);  // PHSEG2 = 3

    // 250 kbps @ 16 MHz
    //mcp_2515_write(MCP_CNF1, 0x01);  // SJW = 1, BRP = 1
    //mcp_2515_write(MCP_CNF2, 0x90 | (3 << 3) | 2); // BTLMODE=1, PHSEG1=3, PROPSEG=2 => 0x9A
    //mcp_2515_write(MCP_CNF3, 0x02);  // PHSEG2 = 3
    
        // 125 kbps @ 16 MHz
    //mcp_2515_write(MCP_CNF1, 0x03);  // SJW = 1, BRP = 3
    //mcp_2515_write(MCP_CNF2, 0x90 | (3 << 3) | 2); // BTLMODE=1, PHSEG1=3, PROPSEG=2 => 0x9A
    //mcp_2515_write(MCP_CNF3, 0x02);  // PHSEG2 = 3
    
    uint8_t CNF1=0x43;
    uint8_t CNF2=0xb5;
    uint8_t CNF3=0x01;       
    mcp_2515_write(MCP_CNF1,CNF1);
    mcp_2515_write(MCP_CNF2,CNF2);
    mcp_2515_write(MCP_CNF3,CNF3);
    

    // Enable RX interrupts (optional)
    mcp_2515_write(MCP_CANINTE, 0x03); // RX0IE and RX1IE enabled

    // Clear error and TX flags
    mcp_2515_bit_modify(MCP_EFLG, 0xFF, 0x00);
    mcp_2515_bit_modify(MCP_TXB0CTRL, 0x08, 0x00); // Clear TXREQ bit

    // Set requested mode (e.g., NORMAL)
    mcp_2515_set_mode(mode);
    _delay_ms(10);

    val = mcp_2515_read(MCP_CANSTAT);
    mode_bits = (val & MODE_MASK);
    if (mode_bits != mode) {
        printf("MCP2515 is NOT in requested mode after configuration! Mode bits = 0x%02X\n", mode_bits);
        return 1;
    }

    //printf("MCP2515 initialized successfully at 500 kbps.\n");
    return 0;
}


void mcp_2515_reset(){
    clear_bit(PORTB, PB1);
    SPI_transfer(MCP_RESET);
    set_bit(PORTB, PB1);
    _delay_ms(10);	// Small delay after reset
}

uint8_t mcp_2515_read(uint8_t address) {
    clear_bit(PORTB, PB1);               // CS low

    SPI_transfer(MCP_READ);              // Send read command (0x03)
    SPI_transfer(address);               // Send register address
    uint8_t data = SPI_transfer(0x00);   // Send dummy byte, read data

    set_bit(PORTB, PB1);                 // CS high

    return data;
}

void mcp_2515_write(uint8_t address, uint8_t data) {
    clear_bit(PORTB, PB1);

    SPI_transfer(MCP_WRITE);     // 0x02
    SPI_transfer(address);
    SPI_transfer(data);

    set_bit(PORTB, PB1);
}

void mcp_2515_request_to_send(char buffer) {
    clear_bit(PORTB, PB1);
    SPI_transfer(buffer);  // Buffer should be MCP_RTS_TX0 / TX1 / TX2 or combination
    set_bit(PORTB, PB1);
}

void mcp_2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    clear_bit(PORTB, PB1);
	
    SPI_transfer(MCP_BITMOD);
    SPI_transfer(address);
    SPI_transfer(mask);
    SPI_transfer(data);
	
    set_bit(PORTB, PB1);
}

uint8_t mcp_2515_read_status() {
    clear_bit(PORTB, PB1);
	
    SPI_transfer(MCP_READ_STATUS);
    uint8_t status = SPI_transfer(0x00);  // Read status byte
	
    set_bit(PORTB, PB1);
	
    return status;
}
