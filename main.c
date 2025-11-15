/*
 * main.c
 *
 * 
 */


#define F_CPU 4915200UL	//This is just a macro.

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "bit_macros.h"
#include "UART_driver.h"

#include "ADC_driver.h"
#include "SRAM_driver.h"
#include "joystick_driver.h"
#include "SPI_driver.h"
#include "OLED_driver.h"
#include "OLED_menu.h"
#include "CAN.h"
#include "CAN_controller_driver.h"

int main(void)
{
    UART_init(9600);
    adc_init ();
    joystick_init();
    oled_init();
	oled_menu_init();
    can_message joy_pos;
	joystick_position_t position;
    can_init(MODE_NORMAL);
	uint8_t id = 0;
    printf("Starting Ping-Pong Game\r\n");
	while(1){
		position = joystick_get_position();
		joy_pos.data[0] = position.x;
		joy_pos.data[1] = position.y;
        joy_pos.data[2] = joystick_button(JOYSTICKBUTTON);
		joy_pos.length = 3;
		joy_pos.id = id;
       
		printf("x: %d, y: %d , button: %d \r\n", joy_pos.data[0], joy_pos.data[1],  joy_pos.data[2]);

		can_message_send(&joy_pos);
		_delay_ms(50);
	}
    
}
