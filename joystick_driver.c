#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

#include "joystick_driver.h"
#include "ADC_driver.h"
#include "bit_macros.h"


volatile joystick_position_t position;
volatile int x_pos;
volatile int y_pos;
volatile sliders_position_t sliders;
volatile state contr_state;

uint8_t joy_x_V_min = 0;
uint8_t joy_x_V_max = 255;
uint8_t joy_y_V_min = 0;
uint8_t joy_y_V_max = 255;

volatile int x_offset;
volatile int y_offset;

uint8_t slide_left_V_min = 0;
uint8_t slide_left_V_max = 255;
uint8_t slide_right_V_min = 0;
uint8_t slide_right_V_max = 255;


// Reads joystick X/Y axes and slider values via ADC, normalizes them, and updates global position variables.
void read_joystick_values() {

    char data_char;
    int32_t data;
    state contr_state = JOYSTICK_X_AXIS;
    
    for (char i = 0; i < 4; i++)
        switch (contr_state) {
            case(JOYSTICK_X_AXIS):
                data_char = ADC_read(CHANNEL0);
                data = (int32_t) data_char;
                x_pos = ((data - x_offset) * 200) / (joy_x_V_max - joy_x_V_min);
                contr_state = JOYSTICK_Y_AXIS;
                break;

            case(JOYSTICK_Y_AXIS):
                data_char = ADC_read(CHANNEL1);
                data = (int32_t) data_char;
                y_pos = ((data - y_offset) * 200) / (joy_y_V_max - joy_y_V_min);
                contr_state = LEFT_SLIDER;
                break;

            case(LEFT_SLIDER):
                data_char = ADC_read(CHANNEL2);
                data = (int32_t) data_char;
                sliders.left = (data * 200) / (slide_left_V_max - slide_left_V_min) - 100;
                contr_state = RIGHT_SLIDER;
                break;

            case(RIGHT_SLIDER):
                data_char = ADC_read(CHANNEL3);
                data = (int32_t) data_char;
                sliders.right = (data * 200) / (slide_right_V_max - slide_right_V_min) - 100;
                break;
        }
}

// Performs an automatic joystick calibration by sampling current X/Y as center offsets.
void joystick_autocalibrate() {
    x_offset = ADC_read(CHANNEL0);
    y_offset = ADC_read(CHANNEL1);
}

// Initializes joystick input pins, enables pull-up on joystick button, calibrates, and reads initial values.
void joystick_init() {
    clear_bit(DDRD, PD3); 
    set_bit(PORTD, PD3); 
    joystick_autocalibrate();   // <-- corrected name
    read_joystick_values();
}

// Placeholder for manual joystick calibration (currently not implemented).
void joystick_manual_calibrate() {
    return;
}

// Returns button state: 1 if pressed, 0 if released, or EXIT_FAILURE if the button ID is invalid.
int joystick_button(usb_button_t button) {

    switch (button) {
        case JOYSTICK_BUTTON:
            return !test_bit(PIND, PIND3);

        case L_BUTTON:
            return test_bit(PINB, PINB2);

        case R_BUTTON:
            return test_bit(PINB, PINB1);

        default:
            printf("Not a valid button retry");
            return EXIT_FAILURE;
    }

}

// Reads and returns the current joystick position as an (x, y) struct using the latest ADC values.
joystick_position_t joystick_read_position() {
    joystick_position_t return_pos;

    read_joystick_values();
    return_pos.x = x_pos;
    return_pos.y = y_pos;
    return return_pos;
}

// Converts the current joystick position into a direction enum (UP, DOWN, LEFT, RIGHT, or NEUTRAL).
joystick_direction_t joystick_read_direction() {

    joystick_position_t current_pos = joystick_read_position();
    int x = x_pos;
    int y = y_pos;

    if (abs(x) >= abs(y)) {
        if (abs(x) < 10) {
            return NEUTRAL;
        } 
        else if (x < 0) {
            return LEFT;
        } 
        else {
            return RIGHT;
        }
    }
    else {
        if (abs(y) < 65) {
            return NEUTRAL;
        } 
        else if (y < 0) {
            return DOWN;
        } 
        else {
            return UP;
        }
    }
}

// Reads and returns the current left slider position (scaled value).
int slider_read_left(void) {
    read_joystick_values();
    return sliders.left;
}

// Reads and returns the current right slider position (scaled value).
int slider_read_right(void) {
    read_joystick_values();
    return sliders.right;
}
