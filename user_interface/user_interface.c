#include "user_interface.h"

void ui_clear_screen() {
    system(CLEAR_SCREEN);
}

void ui_print_menu() {
	ui_clear_screen();
    printf("\n\t%s\n", "Options");
    printf("\n\t%d: %s\n", 1, "List celestial body's data");
    printf("\n\t%d: %s\n", 2, "List celestial bodies that are planets");
    printf("\n\t%d: %s\n", 3, "List celestial body's satellites");
    printf("\n\t%d: %s\n", 0, "Exit");
}

void ui_print_body(void *body) {
	Body *aux_body = (Body *)body;
	printf("\n\t%sId (french name):%s\t%s%s%s\n", ANSI_RED_BACKGROUND, ANSI_RESET, ANSI_RED_BACKGROUND, aux_body->id, ANSI_RESET);
	printf("\n\tName:\t\t\t");
	// Some bodies have empty names, in those cases their id is printed.
	aux_body->name[0] ? printf("%s\n", aux_body->name) : printf("%s\n", aux_body->id);
    printf("\n\tIs it a planet\?\t\t");
    aux_body->is_planet ? printf("Yes\n") : printf("No\n");
	printf("\n\t%sSatellites (ids):%s\n", ANSI_CYAN_TEXT, ANSI_RESET);
	for (size_t i = 0; i < aux_body->moons_count; i++)
		printf("\n\t\t\t\t%s%s%s\n", ANSI_CYAN_TEXT, aux_body->moons[i], ANSI_RESET);
    printf("\n\tRadius:\t\t\t%d km\n", aux_body->radius);
    printf("\n\tMass:\t\t\t");
	// Some bodies have null mass objects, in those cases mass_value and mass_exponent
	// are both assigned zero and aren't printed. mass_exponent is checked because it's
	// an int, while mass_value is a float. There are no bodies with zero as the
	// mass_exponent value besides the ones with null mass objects, so there's no problem.
	aux_body->mass_exponent ? printf("%.5f x 10^%d kg\n", aux_body->mass_value, aux_body->mass_exponent) : printf("\n");
    printf("\n\tGravity:\t\t%.5f m/s^2\n", aux_body->gravity);
}

const unsigned char ui_get_char_input(const char *string) {
    printf("\n\t%s", string);
    unsigned char ret = getchar();
    getchar(); // Get the '\n'.
    return ret;
}

const char *ui_get_string_input(const char *string) {
    printf("\n\t%s", string);
    char input_string[30];
    fgets(input_string, sizeof input_string, stdin);
	input_string[strcspn(input_string, "\n")] = 0; // Remove the '\n'.
    return strdup(input_string); // free required.
}

void ui_option_not_valid(const char *string) {
    printf("\n\t%s%s%s\n", ANSI_RED_TEXT, string, ANSI_RESET);
    ui_press_enter_to_continue();
}

void ui_press_enter_to_continue() {
    unsigned char enter = 0;
    printf("\n\n\t%sPress the Enter key to continue...%s ", ANSI_YELLOW_TEXT, ANSI_RESET);
    while ('\n' != enter)
        enter = getchar();
}

void ui_exit(const char *string) {
	printf("\n\t%s%s%s\n\n", ANSI_BLUE_TEXT, string, ANSI_RESET);
}
