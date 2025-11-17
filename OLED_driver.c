/*
 * OLED_driver.c
 *
 *  Author: Mohamed Abosreea , Kariman Helmy , Marwa Elbadawy
 */
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "SPI_driver.h"
#include "OLED_driver.h"
#include "font_4x6.h"
#include "font_5x7.h"
#include "font_5x7w.h"
#include "font_8x8.h"
#include "bit_macros.h"

static FILE mystdout = FDEV_SETUP_STREAM(oled_put_char, NULL, _FDEV_SETUP_WRITE);
static FILE myinvstdout = FDEV_SETUP_STREAM(oled_inv_put_char, NULL, _FDEV_SETUP_WRITE);

#ifndef OLED_COMMAND_ADDRESS
#define OLED_COMMAND_ADDRESS  0x1000
#endif

#ifndef OLED_DATA_ADDRESS
#define OLED_DATA_ADDRESS 0x1200
#endif

typedef enum{HORIZONTAL_MODE, VERTICAL_MODE, PAGE_MODE} adressing_mode;

volatile char* ext_oled_cmd = OLED_COMMAND_ADDRESS;
volatile char* ext_oled_data = OLED_DATA_ADDRESS;

volatile oled_position_t position;

// Checks and wraps the current text drawing position when reaching the OLED edges.
void oled_is_out_of_bounds() {
	if (position.col > 127) {
		position.col -= 128;
		position.row += 1;
		if (position.row > 7) {
			position.row = 0;
		}
	}
}

// Sends a single command byte to the OLED controller over SPI.
void write_command(uint8_t command){
	// Select sending command to the data-commant line
    clear_bit(PORTD,PD0);
	// Activate OLED chip select
    clear_bit(PORTB, PB4);
    SPI_write(command);
	// deactivate OLED chip select
    set_bit(PORTB, PB4);

}

// Sends a single data byte to the OLED controller over SPI.
void write_data(uint8_t data){
	// Select sending data to the data-commant line
    set_bit(PORTD,PD0);
	// Activate OLED chip select
    clear_bit(PORTB, PB4);
	SPI_write(data);
	// deactivate OLED chip select
    set_bit(PORTB, PB4);
}

// Writes a character in normal mode.
int oled_put_char(unsigned char c){
	uint8_t printChar = c-32;
	
	for (int i=0; i < fontSize; i++) {
		write_data(pgm_read_word(&font_8x8[printChar][i]));
		position.col += fontSize;
		oled_is_out_of_bounds();
	}
	
	return 0;
}

// Writes a character in dark mode (inverted pixels).
int oled_inv_put_char(unsigned char c){
	uint8_t printChar = c-32;
	
	for (int i=0; i < fontSize; i++) {
		write_data(~pgm_read_word(&font_8x8[printChar][i]));
		position.col += fontSize;
		oled_is_out_of_bounds();
	}
	
	return 0;
}

// Sets the memory addressing mode of the OLED (horizontal, vertical, or page).
void oled_set_adressing_mode(adressing_mode mode) {
	write_command(0x20);
	write_command(mode);
}

// Prints formatted text in normal mode.
void oled_printf(char* data, ...){
	va_list args;
	va_start(args, data);
	vfprintf(&mystdout, data, args);
	va_end(args);
	
}

// Prints formatted text in dark mode.
void oled_inv_printf(char* data, ...){
	va_list args;
	va_start(args, data);
	vfprintf(&myinvstdout, data, args);
	va_end(args);
	
}

// Returns 1 if the given string is too long to fit in a single OLED line with current font size.
int long_string(char* data){
	if (strlen(data)*fontSize > 128){
		return 1;
	}
	return 0;
}

// Moves the cursor so that the given title string is horizontally centered on the screen.
void oled_align_centre(char* title) {
	oled_goto_column(64 - fontSize*strlen(title)/2);
}

// Initializes the OLED display, SPI, GPIO lines, and basic display parameters.
void oled_init(){
    SPI_init();
    set_bit(DDRD, PD0);      // PD0 is used for Data/Command line
    set_bit(DDRB , PB4);     // slave select for the OLED

    write_command(0xAE);     // Display OFF

    write_command(0xA1);     // Segment remap: column 127 -> SEG0 (horizontal flip)
    write_command(0xC8);     // COM output scan direction: COM63 to COM0 (vertical flip)

    write_command(0xDA);     // Set COM pins hardware configuration
    write_command(0x12);     // Alternative COM pin config, disable left/right remap

    write_command(0xA8);     // Set multiplex ratio
    write_command(0x3F);     // 64 MUX (63 + 1)

    write_command(0xD5);     // Set display clock divide ratio / oscillator frequency
    write_command(0x80);     // Max oscillator freq (1000b), divide ratio = 1 (0000b)

    write_command(0x81);     // Set contrast control
    write_command(0x50);     // Medium contrast (adjust as needed)

    write_command(0xD9);     // Set pre-charge period
    write_command(0x22);     // Phase 1 = 2 DCLK, Phase 2 = 2 DCLK (reset value)

    // Set addressing mode to horizontal
    write_command(0x20);     // Set Memory Addressing Mode
    write_command(0x00);     // 00b = Horizontal Addressing Mode

    write_command(0xDB);     // Set VCOMH deselect level
    write_command(0x34);     // 0.78 * VCC (recommended reset value)

    // Note: Command 0xAD (Master Configuration) is NOT supported on SSD1309 ? removed!

    write_command(0xA4);     // Output follows RAM content (not forced ON)
    write_command(0xA6);     // Normal display 
    write_command(0xAF);     // Display ON
}

// Moves the OLED cursor to the specified text line (0~7) without changing the column.
void oled_goto_line(int line){
	if (line > 7 || line < 0) {
		return 0;
	}
	else {
		position.row = line;
		oled_set_adressing_mode(PAGE_MODE);
		write_command(0xB0 + line);
		oled_set_adressing_mode(HORIZONTAL_MODE);
	}
}

// Moves the OLED cursor to the specified pixel column (0~127) without changing the row.
void oled_goto_column(int column){
	if (column > 127 || column < 0) {
		return 0;
	}
	else {
		position.col = column;
		oled_set_adressing_mode(PAGE_MODE);
		
		int numLow = column % 16;
		int numHigh = column / 16;
		
		write_command(numLow);
		write_command(16 + numHigh);
		oled_set_adressing_mode(HORIZONTAL_MODE);
	}
	
}

// Moves the OLED cursor to a given character position on the current line based on font size.
void oled_goto_letter(int letterPlace){
	if (letterPlace <= 0) {
		oled_goto_column(0);
	}
	else {
		oled_goto_column(letterPlace*fontSize - 1);
	}
}

// Clears  all pixels in the specified line.
void oled_clear_line(int line){
	oled_pos(line, 0);
	
	for (int col = 0; col < 128; col++) {
		write_data(0b00000000);
	}
	oled_goto_line(line);
}

// Fills the specified line with all pixels set .
void oled_fill_line(int line){
	oled_pos(line, 0);
	
	for (int col = 0; col < 128; col++) {
		write_data(~0b00000000);
	}
	oled_goto_line(line);
}

// Sets the OLED cursor to the given row and column position.
void oled_pos(int row, int column){
	oled_goto_line(row);
	oled_goto_column(column);	

}
