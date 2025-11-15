/*
 * OLED_menu.h
 *
 *  Author: Mohamed Abosreea , Kariman Helmy , Marwa Elbadawy
 */

#ifndef OLED_MENU_H_
#define OLED_MENU_H_

#include "joystick_driver.h"

typedef struct{
	char* title;
	struct menu_t* parent;
	struct menu_t* first_child;
	struct menu_t* right_sibling;
} menu_t;

typedef enum{
	NONE,
	PLAY_GAME,
	HIGHSCORE
} menu_option_t;

menu_option_t oled_menu_selection(void);
menu_t *oled_menu_init(void);
void oled_menu_print(menu_t *menu);

#endif /* OLED_MENU_H_ */