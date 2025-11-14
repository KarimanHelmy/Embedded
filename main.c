/*
 * main.c
 *
 * 
 */


#define F_CPU 4915200UL	//This is just a macro, it has no data type.

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
//#include "OLED_driver.h"
//#include "OLED_menu.h"
#include "CAN.h"
#include "CAN_controller_driver.h"
/*int main()
{    
     printf("Hello from MPLAB XC8!\r\n");

    UART_init(9600);
    DDRA = 0xFF ;
    
    while(1)
    {
        put_char('A', stdout);
        
        _delay_ms(1000);
        
        if(get_char(stdin) == 'B')
        {
            PORTA  = ~PORTA ;
        }
    }
    return 0;
}*/

/*int main() {
    DDRA = 0xFF;    // Set all PORTA pins as outputs
PORTA = 0x01;   // Send known pattern

  DDRE = 0b10;        // ALE as output
DDRA = 0xFF;        // PORTA as output

// 1. Latch 0x01
PORTA = 0x01;       // Only Q0 should turn ON
PORTE |= (1 << PE1); // ALE high
_delay_ms(100);
PORTE &= ~(1 << PE1); // ALE low - store the value
_delay_ms(2000);

// 2. Latch 0xAA
PORTA = 0xAA;       // Binary: 10101010
PORTE |= (1 << PE1); // ALE high
_delay_ms(100);
PORTE &= ~(1 << PE1); // ALE low - store new value



    return 0;
}*/
/*
int main() {
    UART_init(9600);
    put_char(0x41,stdin);
    SRAM_init();
    int state = 10;
    state = SRAM_write(0xFF, 50);
    printf("state is %d", state);
    char value = 10 ;
    value = SRAM_read(0xFF);
    printf("value is %d", value);
    while(1)
    {
        SRAM_test();
        _delay_ms(1000);
    }
    
    return 0;
}*/
/*
int main()
{
    //joystick_position_t position;
    //int prescaler_joystick_timer = 1024;
	//joystick_direction_t direction;
    UART_init(9600);
    put_char(0x41,stdin);
    printf("Hello World\n\r");
   // SRAM_init();
    //ADC_init();
    adc_init ();
    uint8_t ch1 = 100;
    uint8_t ch2= 100;
    uint8_t ch3= 100;
    uint8_t ch4= 100;
    uint8_t arr[4] = {100,100,100,100};
    while(1)
    {

       ch1 = adc_read(0);
       printf("ch0 is %d ", ch1);
       ch2 = adc_read(1);
       printf("ch1 is %d ", ch2);
       ch3 = adc_read(2);
       printf("ch2 is %d ", ch3);
       ch4 = adc_read(3);
       printf("ch3 is %d \n\r", ch4);
       _delay_ms(1000);
    };
};


*/
/*
int main()
{
    
    UART_init(9600);
    put_char(0x41,stdin);
    printf("Hello World\n\r");
    adc_init ();

    joystick_init();

    joystick_position_t position;
    joystick_direction_t direction;
        while(1){
            //put_char(0x42,stdin);
            //printf("Hello World\n\r");
               printf("Joystick: x: %d \n\r", position.x);
               //_delay_ms(100);
               printf("Joystick: y: %d \n\r", position.y);
               //_delay_ms(100);
               position = joystick_get_position();

               
/*

                if (joystick_button(JOYSTICKBUTTON)){
                        printf("JOYSTICK BUTTON\n\r");
                }

                if (joystick_button(LBUTTON)){
                        printf("LEFT BUTTON\n\r");
                }

                if (joystick_button(RBUTTON)){
                        printf("RIGHT BUTTON\n\r");
                }


                direction = joystick_get_direction();

                printf("Direction: %d\n\r", direction);
                _delay_ms(100);
        };
        
};*/

/* Exercise  4 and 5*/
/*
int main(void) {
    UART_init(9600);
    can_init(MODE_LOOPBACK);
    
    can_message msg1 = { .id = 100, .length = 5, .data = {'H','e','l','l','o'} };
    can_message msg2 = { .id = 200, .length = 5, .data = {'W','o','r','l','d'} };

    
	while(1){

    // Send and receive msg1
    can_message_send(&msg1);
    _delay_ms(20);

    if (can_interrupt()) {
        can_message r1 = can_handle_messages();
        printf("Received msg1: ID=%u, Data=", r1.id);
        for (int i = 0; i < r1.length; i++) {
            printf("%c", r1.data[i]);
        }
        printf("\n");
    }
    _delay_ms(100);
    // Send and receive msg2
    can_message_send(&msg2);
    _delay_ms(20);

    if (can_interrupt()) {
        can_message r2 = can_handle_messages();
        printf("Received msg2: ID=%u, Data=", r2.id);
        for (int i = 0; i < r2.length; i++) {
            printf("%c", r2.data[i]);
        }
        printf("\n");
        
    }
    _delay_ms(100);
        /*
		oled_menu_selection();
        clear_bit(PORTD , DD1); // IO slave selec
        SPI_write(0x05);
        _delay_us(50);
        SPI_write(0x05);
        _delay_us(50);
        SPI_write(0x05);
        _delay_us(50);
        set_bit(PORTD , DD1);
        
        _delay_ms(1000);
        
        clear_bit(PORTD , DD1); // IO slave selec
        SPI_write(0x05);
        _delay_us(50);
        SPI_write(0x05);
        _delay_us(50);
        SPI_write(0x00);
        _delay_us(50);
        set_bit(PORTD , DD1);
        
        _delay_ms(1000);
		//_delay_ms(200);*/


        //oled_menu_selection();
 //   }
//}

/*
int main()
{   
    UART_init(9600);
    //sei();
    adc_init ();
    joystick_init(); 
    oled_init();
	oled_menu_init();
    //set_bit(DDRD , PD1); // slave select for the IO Board
    //set_bit(PORTD , DD1);
    //set_bit(DDRB , PB4); // slave select for the OLED
    //set_bit(PORTB , PB4);    
    can_init(MODE_LOOPBACK);
	can_message test;
    can_message test_return ;
	test.id		= 1337;
	test.data[0]	= 'H';
	test.data[1]	= 'e';
	test.data[2]	= 'l';
	test.data[3]	= 'l';
	test.data[4]	= 'o';
	test.length	= 5;

	can_message test2;
	test2.id		= 1338;
	test2.data[0]	= 'W';
	test2.data[1]	= 'o';
	test2.data[2]	= 'r';
	test2.data[3]	= 'l';
	test2.data[4]	= 'd';
	test2.length	= 5;


	can_message_send(&test);
	can_message_send(&test2);

	while(1){
        printf("Sucessful\r\n");
        oled_menu_selection();
        if ( can_interrupt()){
			can_handle_messages();
		}
		_delay_ms(30);
		//_delay_ms(1000);
	}
}
*/

int main(void)
{
    UART_init(9600);
    adc_init ();
    joystick_init();
    oled_init();
	oled_menu_init();
    put_char('A',stdout);
    joystick_direction_t direction;
    can_message joy_pos;
	joystick_position_t position;
   // can_init(MODE_NORMAL);
	uint8_t id = 0;
    printf("Starting\r\n");
	while(1){
        printf("Hello World\r\n");
        oled_init();
	    oled_menu_init();
		position = joystick_get_position();
		joy_pos.data[0] = position.x;
		joy_pos.data[1] = position.y;
        joy_pos.data[2] = joystick_button(JOYSTICKBUTTON);
       // joy_pos.data[2] = 0;
		joy_pos.length = 3;
		joy_pos.id = id;
       
		printf("x: %d, y: %d , button: %d \r\n", joy_pos.data[0], joy_pos.data[1],  joy_pos.data[2]);

		//can_message_send(&joy_pos);
       // _delay_ms(100);
		//id = id + 1;
        //printf("x*************************** \r\n");
		_delay_ms(500);
	}
    
}
