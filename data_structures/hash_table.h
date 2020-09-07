#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>
#include "list.h"

typedef struct Htable {
	List_node **base;
	size_t size;
	int (*hash_func)(const void *);
	int (*key_cmp_func)(const void *, const void *, void *);
} Htable;

Htable *htable_create(size_t size, int (*hf)(const void *), int (*kcf)(const void *, const void *, void *));
void htable_init(Htable *htable, size_t size, int (*hf)(const void *), int (*kcf)(const void *, const void *, void *));
void htable_insert(Htable *htable, void *data);
void htable_remove(Htable *htable, const void *key);
void *htable_lookup(Htable *htable, const void *key);
void htable_foreach(Htable *htable, void (*func)(void *, void *), void *context);
size_t htable_size(Htable *htable);
void htable_destroy(Htable *htable);

#endif
