#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "json_manipulation.h"

json_t *jsonm_get_data(const char *url) {
	Custom_buffer *buffer = urlt_transfer_data(url);
	if (NULL == buffer) {
		fprintf(stderr, "\nError: http_get returned NULL.\n");
		return NULL;
	}
	json_t *root = json_loads(buffer->data, 0, NULL);
	if (json_is_null(root))
		fprintf(stderr, "\nError: json_is_null returned true.\n");
	free(buffer->data);
	return root;
}

int jsonm_load_bodies(json_t *reference, int (*insert_body)(Body *), void (*free_body)(void *, void *)) {
	json_t *bodies_array = json_object_get(reference, "bodies");
	if (json_is_null(bodies_array)) {
		fprintf(stderr, "\nError: json_is_null returned true.\n");
		return 1;
	}
	size_t index;
	json_t *value;
	json_array_foreach(bodies_array, index, value) {
		Body *body = malloc(sizeof *body);
		if (NULL == body) { // No memory available.
			fprintf(stderr, "\n%s\n", strerror(errno));
			return 1;
		}
		body->id = strdup(json_string_value(json_object_get(value, "name")));
		body->name = strdup(json_string_value(json_object_get(value, "englishName")));
		body->is_planet = json_boolean_value(json_object_get(value, "isPlanet"));
		body->moons = NULL;
		body->moons_count = 0;
		json_t *moons_array = json_object_get(value, "moons");
		if (!json_is_null(moons_array)) {
			body->moons_count = json_array_size(moons_array);
			body->moons = malloc(sizeof *body->moons * body->moons_count);
			if (NULL == body->moons) { // No memory available.
				fprintf(stderr, "\n%s\n", strerror(errno));
				free_body(body, NULL);
				return 1;
			}
			size_t _index;
			json_t *_value;
			json_array_foreach(moons_array, _index, _value) {
				body->moons[_index] = strdup(json_string_value(json_object_get(_value, "moon")));
			}
		}
		body->radius = json_real_value(json_object_get(value, "meanRadius"));
		json_t *mass_object = json_object_get(value, "mass");
		// If the mass object is null, mass_value and mass_exponent are both assigned zero,
		// so a json_is_null verification isn't necessary.
		body->mass_value = json_real_value(json_object_get(mass_object, "massValue"));
		body->mass_exponent = json_integer_value(json_object_get(mass_object, "massExponent"));
		body->gravity = json_real_value(json_object_get(value, "gravity"));
		if (insert_body(body)) // body is already inside the collection.
			free_body(body, NULL);
	}
	return 0;
}

void jsonm_free_reference(json_t *reference) {
	json_decref(reference); // Free the json_t object or array.
}
