/*
 * CAN_controller_driver.c
 *
 * Created: 13.10.2025
 *  Author: Mohamed Abosreea , Kariman Helmy , Marwa Elbadawy
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

uint8_t mcp_2515_init(uint8_t mode) {
    // CAN CS pin as output
    set_bit(DDRB, PB1); 

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
    /* CAN Speed at 250 Kbps */
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
    // Read current state
    val = mcp_2515_read(MCP_CANSTAT);
    mode_bits = (val & MODE_MASK);
    if (mode_bits != mode) {
        printf("MCP2515 is NOT in requested mode after configuration! Mode bits = 0x%02X\n", mode_bits);
        return 1;
    }
    printf("MCP2515 initialized successfully at 250 kbps.\n");
    return 0;
}


void mcp_2515_reset(){
    mcp_activate_slave();
    SPI_transfer(MCP_RESET);
    mcp_deactivate_slave();
    _delay_ms(10);	// Small delay after reset
}

uint8_t mcp_2515_read(uint8_t address) {
    mcp_activate_slave();                // CS low
    SPI_transfer(MCP_READ);              // Send read command (0x03)
    SPI_transfer(address);               // Send register address
    uint8_t data = SPI_transfer(0x00);   // Send dummy byte, read data

    mcp_deactivate_slave();                  // CS high
    return data;
}

void mcp_2515_write(uint8_t address, uint8_t data) {
    mcp_activate_slave();
    SPI_transfer(MCP_WRITE);     // 0x02
    SPI_transfer(address);
    SPI_transfer(data);
    mcp_deactivate_slave();
}

void mcp_2515_request_to_send(char buffer) {
    mcp_activate_slave();
    SPI_transfer(buffer);  // Buffer should be MCP_RTS_TX0 / TX1 / TX2 or combination
    mcp_deactivate_slave();
}

void mcp_2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    mcp_activate_slave();
    SPI_transfer(MCP_BITMOD);
    SPI_transfer(address);
    SPI_transfer(mask);
    SPI_transfer(data);	
    mcp_deactivate_slave();
}
