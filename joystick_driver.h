/*
 * joystick_driver.h
 */ 

#ifndef JOYSTICK_DRIVER_H_
#define JOYSTICK_DRIVER_H_

void joystick_init();
void joystick_manual_calibration(void);

typedef enum {JOYSTICK_X_AXIS, JOYSTICK_Y_AXIS, LEFT_SLIDER, RIGHT_SLIDER} state;

typedef enum {JOYSTICK_BUTTON, L_BUTTON, R_BUTTON} usb_button_t;

int joystick_button(usb_button_t button);

typedef struct {
	int left, right;
} sliders_position_t;

typedef enum {LEFT, RIGHT, UP, DOWN, NEUTRAL} joystick_direction_t;
	
typedef struct {
	int x;
	int y;
} joystick_position_t;

joystick_position_t joystick_read_position(void);
joystick_direction_t joystick_read_direction(void);

int slider_read_left(void);
int slider_read_right(void);

#endif /* JOYSTICK_DRIVER_H_ */
