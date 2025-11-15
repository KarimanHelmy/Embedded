/*
 * OLED_menu.c
 *
 *  Author: Mohamed Abosreea , Kariman Helmy , Marwa Elbadawy
 */ 

#include "OLED_driver.h"
#include "OLED_menu.h"

#include <string.h>
#include <stdio.h>

volatile int current_line = 2;
volatile int current_menu_size;

volatile menu_t *current_menu;

int cutter = 0;
int title_cutter = 0;
int display_line_offset = 0;

menu_t* new_menu(char* name, menu_t* parent) {
	menu_t* menu = malloc(sizeof(menu_t));
	menu->title = name;
	menu->parent = parent;
	menu->right_sibling = NULL;
	menu->first_child = NULL;
	return menu;
}

void set_right_sibling(menu_t* menu, menu_t* right_sibling) {
	menu->right_sibling = right_sibling;
}

void set_first_child(menu_t* menu, menu_t* first_child) {
	menu->first_child = first_child;
}

menu_t *oled_menu_init(void) {
	menu_t* main_menu = new_menu("Main Menu", NULL);
	menu_t* highscores = new_menu("Highscores", main_menu);
	menu_t* play_game = new_menu("Play Game", main_menu);
	
	set_first_child(main_menu, play_game);
	set_right_sibling(play_game, highscores);
	
	current_menu = main_menu;
	current_menu_size = size_of_menu(current_menu);
	oled_menu_print(current_menu);

	return current_menu;
}

menu_option_t oled_menu_selection() {
	
	joystick_direction_t direction = joystick_get_direction();
	
	if (direction == UP) {
		cutter = 0;
		title_cutter = 0;
		current_line--;
		if (current_line < 2) {
			current_line = current_line+current_menu_size;
			if (current_menu_size > 6){
				display_line_offset = current_menu_size - 5;
			}
		}
		if (current_line < display_line_offset + 2){
			display_line_offset --;
		}
		oled_menu_print(current_menu);
		return NONE;
	}
	else if (direction == DOWN) {
		cutter = 0;
		title_cutter = 0;
		current_line++;
		if (current_line >= 2 + current_menu_size) {
			current_line = 2;
			display_line_offset = 0;
		}
		if (current_line > 6){
			display_line_offset = current_line - 6;
		}
		oled_menu_print(current_menu);
		return NONE;
	}
	else
	{
		/*Do Nothing*/
	}
}


int size_of_menu(menu_t* menu) {
	
	int menu_size = 0;
	
	menu = menu->first_child;
	if (menu == NULL) {
		return menu_size;
	}
	menu_size ++;											
	while (menu->right_sibling != NULL) {
		menu = menu->right_sibling;		
		menu_size++;
	}
	return menu_size;	
}

void printTitle(char* title){
	oled_align_centre(title);
	int i = 0;
	while(title[i])
	{
		unsigned char c = toupper(title[i]);
		
		oled_put_char(c);
		i++;
	}
	oled_pos(1,0);
	for (int i = 0; i < 16; i++) {
		oled_printf("-");
	}
	oled_pos(2,0);
}

void oled_menu_print(menu_t *menu) {
	
	if (cutter < -1){
		cutter ++;
		return;
	}
	if (title_cutter < -1){
		title_cutter ++;
		return;
	}
	
	oled_reset();
	
	if (title_cutter == -1){
		title_cutter ++;
	}
	
	if (long_string(menu->title + title_cutter)){
		title_cutter ++;
		printTitle(menu->title + title_cutter);
	}
	
	else if(title_cutter > 0){
		printTitle(menu->title);
		title_cutter = -5;
		
	}
	else{
		printTitle(menu->title);
	}
	
	int line = 2;
	
	menu = menu->first_child;
	
	for (int i = 0; i < display_line_offset; i++){
		menu = menu->right_sibling;
	}
	
	
	while (menu != NULL && line < 7) {
		
		oled_pos(line,0);
		
		if (current_line == line + display_line_offset) {
			
			oled_fill_line(line);
			
			if (cutter == -1){
				cutter ++;
			}
			
			if (long_string(menu->title + cutter)){
				
				cutter ++;
				oled_inv_printf(menu->title + cutter);
			}
			
			else if(cutter > 0){
				oled_inv_printf(menu->title);
				cutter = -5;
			}
			else{
				oled_inv_printf(menu->title);
			}
		}else {
			oled_printf(menu->title);
		}
		++line;
		menu = menu->right_sibling;
	}
	
}
