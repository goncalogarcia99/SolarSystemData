// This file contains the definition of the Body type.

#ifndef BODY_H
#define BODY_H

#include <stddef.h>

typedef struct Body {
	// The body's name in french is used as its id instead of the the real id because the
	// moons array contains french names, not ids. This way it'll be easier to find a specific moon.
	const char *id; // The name string of a body object - the body's name in french.
	const char *name; // The englishName string of a body object - the body's name in english.
	unsigned char is_planet;
	const char **moons;
	size_t moons_count;
	int radius; // km
	float mass_value; // kg
	int mass_exponent;
	float gravity; // m/s^2
} Body;

#endif
