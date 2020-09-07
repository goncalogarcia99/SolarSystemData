#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../body.h" // Body struct.

#define CLEAR_SCREEN "clear"

#define ANSI_RED_BACKGROUND		"\e[41m"
#define ANSI_RED_TEXT       	"\x1b[31m"
#define ANSI_BLUE_TEXT      	"\x1b[34m"
#define ANSI_YELLOW_TEXT    	"\x1b[33m"
#define ANSI_CYAN_TEXT			"\x1b[36m"
#define ANSI_RESET          	"\x1b[0m"

void ui_clear_screen();
void ui_print_menu();
void ui_print_body(void *body);
const unsigned char ui_get_char_input(const char *string);
const char *ui_get_string_input(const char *string);
void ui_option_not_valid(const char *string);
void ui_press_enter_to_continue();
void ui_exit(const char *string);

#endif
