#include <errno.h>
#include "web/json_manipulation.h"
#include "data_structures/hash_table.h"
#include "user_interface/user_interface.h"

#define BODIES_HTABLE_SIZE	10000
#define PLANETS_ARRAY_SIZE	5000

typedef struct Bodies {
	Body **array;
	size_t count;
} Bodies;

json_t *reference;
Htable *bodies_htable;

int hash_func(const void *body);
int key_cmp_func(const void *body_1, const void *body_2, void *not_used);
int solar_insert_body(Body *body);
Body *solar_get_body(const char *body_name);
Bodies *solar_get_planets();
void solar_get_planets_aux(void *body, void *planets);
Bodies *solar_get_satellites(const char *body_id);
void free_body(void *body, void *not_used);
int handle_user_option(unsigned char user_option);

int main(int argc, char **argv) {
	reference = jsonm_get_data(argv[1]);
	if (json_is_null(reference)) {
		fprintf(stderr, "\nError: json_is_null returned true.\n");
		return 1;
	}
	bodies_htable = htable_create(BODIES_HTABLE_SIZE, hash_func, key_cmp_func);
	if (NULL == bodies_htable) {
		fprintf(stderr, "\nError: htable_create returned NULL.\n");
		jsonm_free_reference(reference);
		return 1;
	}
	if (jsonm_load_bodies(reference, solar_insert_body, free_body)) {
		fprintf(stderr, "\nError: jsonm_load_bodies failed.\n");
		jsonm_free_reference(reference);
		htable_foreach(bodies_htable, free_body, NULL);
		htable_destroy(bodies_htable);
		return 1;
	}
	jsonm_free_reference(reference);

	unsigned char user_option = 0;
	do {
		ui_print_menu();
        user_option = ui_get_char_input("Enter your choice: ");
	} while (!handle_user_option(user_option));
	htable_foreach(bodies_htable, free_body, NULL);
	htable_destroy(bodies_htable);
	return 0;
}

int hash_func(const void *body) {
	const char *body_id = ((const Body *)body)->id;
	int hash = 0;
	while (*body_id)
		hash += *body_id++;
	return hash;
}

int key_cmp_func(const void *body_1, const void *body_2, void *not_used) {
	return strcmp(((const Body *)body_1)->id, ((const Body *)body_2)->id);
}

int solar_insert_body(Body *body) {
	if (NULL != htable_lookup(bodies_htable, body)) {
		fprintf(stderr, "\nError: %s is already in the collection.\n", body->id);
		return 1;
	}
	htable_insert(bodies_htable, body);
	return 0;
}

Body *solar_get_body(const char *body_id) {
	Body aux_body = {.id = body_id}; // The hash key (the body's id) is embedded in aux_body.
	return htable_lookup(bodies_htable, &aux_body);
}

Bodies *solar_get_planets() {
	Bodies *planets = malloc(sizeof *planets);
	if (NULL == planets) { // No memory available.
		fprintf(stderr, "\n%s\n", strerror(errno));
		return NULL;
	}
	planets->array = malloc(sizeof *planets->array * PLANETS_ARRAY_SIZE);
	if (NULL == planets->array) { // No memory available.
		fprintf(stderr, "\n%s\n", strerror(errno));
		free(planets);
		return NULL;
	}
	planets->count = 0;
	htable_foreach(bodies_htable, solar_get_planets_aux, planets);
	return planets;
}

void solar_get_planets_aux(void *body, void *planets) {
	if (((Body *)body)->is_planet)
		((Bodies *)planets)->array[((Bodies *)planets)->count++] = (Body *)body;
}

Bodies *solar_get_satellites(const char *body_id) {
	Body *body = solar_get_body(body_id);
	if (NULL == body) {
		ui_option_not_valid("There's no such body.");
		return NULL;
	}
	if (NULL == body->moons) {
		ui_option_not_valid("The body has no satellites.");
		return NULL;
	}
	Bodies *satellites = malloc(sizeof *satellites);
	if (NULL == satellites) { // No memory available.
		fprintf(stderr, "\n%s\n", strerror(errno));
		return NULL;
	}
	satellites->array = malloc(sizeof *satellites->array * body->moons_count);
	if (NULL == satellites->array) { // No memory available.
		fprintf(stderr, "\n%s\n", strerror(errno));
		free(satellites);
		return NULL;
	}
	satellites->count = body->moons_count;
	for (size_t i = 0; i < satellites->count; i++)
		satellites->array[i] = solar_get_body(body->moons[i]);
	return satellites;
}

void free_body(void *body, void *not_used) {
	Body *aux_body = (Body *)body; // With aux_body only one Body * cast is made.
	for (size_t i = 0; i < aux_body->moons_count; i++)
		free((char *)aux_body->moons[i]); // The char * cast is needed because free takes a void *, not a const void *.
	free((char **)aux_body->moons); // If body->moons is NULL, free does nothing.
	free((char *)aux_body->name);
	free((char *)aux_body->id);
	free(aux_body);
}

int handle_user_option(unsigned char user_option) {
    if ('0' == user_option) {
        ui_exit("Goodbye!");
        return 1;
    }
    if ('1' == user_option) {
        const char *body_id = ui_get_string_input("Enter the id of the celestial body (its french name): ");
		Body *body = solar_get_body(body_id);
		free((char *)body_id);
		if (NULL == body) {
			ui_option_not_valid("There's no such body.");
			return 0;
		}
		ui_press_enter_to_continue();
		ui_clear_screen();
		ui_print_body(body);
		ui_press_enter_to_continue();
        return 0;
    }
    if ('2' == user_option) {
		Bodies *planets = solar_get_planets();
		if (NULL == planets) {
			fprintf(stderr, "\nError: solar_get_planets returned NULL.\n");
			return 0;
		}
		ui_press_enter_to_continue();
		ui_clear_screen();
		for (size_t i = 0; i < planets->count; i++)
			ui_print_body(planets->array[i]);
		free(planets->array);
		free(planets);
		ui_press_enter_to_continue();
        return 0;
    }
    if ('3' == user_option) {
		const char *body_id = ui_get_string_input("Enter the id of the celestial body (its french name): ");
		Bodies *satellites = solar_get_satellites(body_id);
		free((char *)body_id);
		if (NULL == satellites)
			return 0;
		ui_press_enter_to_continue();
		ui_clear_screen();
		for (size_t i = 0; i < satellites->count; i++) {
			ui_print_body(satellites->array[i]);
		}
		free(satellites->array);
		free(satellites);
		ui_press_enter_to_continue();
        return 0;
    }
    ui_option_not_valid("Not a valid option!");
    return 0;
}
