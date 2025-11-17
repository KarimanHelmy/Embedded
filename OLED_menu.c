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

// Creates a new menu node with a title and a pointer to its parent.
menu_t* new_menu(char* name, menu_t* parent) {
	menu_t* menu = malloc(sizeof(menu_t));
	menu->title = name;
	menu->parent = parent;
	menu->right_sibling = NULL;
	menu->first_child = NULL;
	return menu;
}

// Sets the right_sibling pointer of a menu node to link items on the same level.
void set_right_sibling(menu_t* menu, menu_t* right_sibling) {
	menu->right_sibling = right_sibling;
}

// Sets the first_child pointer of a menu node to link to its submenu.
void set_first_child(menu_t* menu, menu_t* first_child) {
	menu->first_child = first_child;
}

// Builds the main menu structure, sets it as current, and prints it on the OLED.
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

// Reads joystick direction and updates the current selected menu line accordingly.
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

// Counts how many items (children) exist in the given menu level.
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

// Prints the menu title centered on the first line with a separator below it.
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
	
// Draws the current menu on the OLED, highlighting the selected item and handling scrolling.
void oled_menu_print(menu_t *menu) {
	
	if (cutter < -1){                          // Wait a few cycles before shifting long option text horizontally
		cutter ++;
		return;
	}
	if (title_cutter < -1){                    // Wait a few cycles before shifting long title text horizontally
		title_cutter ++;
		return;
	}
	
	oled_reset();
	
	if (title_cutter == -1){                   // First frame after the wait period for title scrolling
		title_cutter ++;
	}
	
	if (long_string(menu->title + title_cutter)){   // If the title with current offset is still too long, keep scrolling
		title_cutter ++;
		printTitle(menu->title + title_cutter);
	}
	
	else if(title_cutter > 0){                 // Title finished scrolling, reset to full title and pause
		printTitle(menu->title);
		title_cutter = -5;
		
	}
	else{                                      // Normal case: title fits on screen, no scrolling
		printTitle(menu->title);
	}
	
	int line = 2;
	
	menu = menu->first_child;
	
	for (int i = 0; i < display_line_offset; i++){
		menu = menu->right_sibling;
	}
	
	
	while (menu != NULL && line < 7) {
		
		oled_pos(line,0);
		
		if (current_line == line + display_line_offset) {   // This is the currently selected menu item
			
			
			oled_fill_line(line);
			
			if (cutter == -1){                 // First frame after the wait period for option text scrolling
				cutter ++;
			}
			
			if (long_string(menu->title + cutter)){   // If the option text is too long, scroll it horizontally
				cutter ++;
				oled_inv_printf(menu->title + cutter);
			}
			
			else if(cutter > 0){               // Option text finished scrolling, show full text and pause
				oled_inv_printf(menu->title);
				cutter = -5;
			}
			else{                              // Normal case: selected option fits, no scrolling
				oled_inv_printf(menu->title);
			}
		}else {                                // Non-selected menu items (printed normally)
			oled_printf(menu->title);
		}
		++line;
		menu = menu->right_sibling;
	}
	
}


