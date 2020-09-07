#ifndef JSON_MANIPULATION_H
#define JSON_MANIPULATION_H

#include <jansson.h>
#include "url_transfer.h"
#include "../body.h" // Body struct.

json_t *jsonm_get_data(const char *url);
int jsonm_load_bodies(json_t *reference, int (*insert_body)(Body *), void (*free_body)(void *, void *));
void jsonm_free_reference(json_t *reference);

#endif
