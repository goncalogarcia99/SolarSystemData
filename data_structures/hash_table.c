#include "hash_table.h"

Htable *htable_create(size_t size, int (*hf)(const void *),
						int (*kcf)(const void *, const void *, void *)) {
	Htable *htable = malloc(sizeof *htable);
	if (NULL == htable) // No memory available.
		return NULL;
	htable->base = malloc(sizeof *htable->base * size);
	if (NULL == htable->base) { // No memory available.
		free(htable);
		return NULL;
	}
	htable_init(htable, size, hf, kcf);
	return htable;
}

void htable_init(Htable *htable, size_t size,
					int (*hf)(const void *),
					int (*kcf)(const void *, const void *, void *)) {
	for (size_t i = 0; i < size; i++)
		htable->base[i] = NULL;
	htable->size = size;
	htable->hash_func = hf;
	htable->key_cmp_func = kcf;
}

void htable_insert(Htable *htable, void *data) {
	size_t index = htable->hash_func(data) % htable->size;
	htable->base[index] = list_insert(htable->base[index], data);
}

void htable_remove(Htable *htable, const void *key) {
	size_t index = htable->hash_func(key) % htable->size;
	if (NULL != htable->base[index]) {
		List_node *list_node = list_lookup(htable->base[index], key, htable->key_cmp_func);
		if (NULL != list_node)
			htable->base[index] = list_remove(htable->base[index], list_node);
	}
}

void *htable_lookup(Htable *htable, const void *key) {
	size_t index = htable->hash_func(key) % htable->size;
	if (NULL == htable->base[index])
		return NULL;
	List_node *list_node = list_lookup(htable->base[index], key, htable->key_cmp_func);
	if (NULL == list_node)
		return NULL;
	return list_data(list_node);
}

void htable_foreach(Htable *htable, void (*func)(void *, void *), void *context) {
	for (size_t i = 0; i < htable->size; i++)
		if (NULL != htable->base[i])
			list_foreach(htable->base[i], func, context);
}

size_t htable_size(Htable *htable) {
	size_t size = 0;
	for (size_t i = 0; i < htable->size; i++)
		if (NULL != htable->base[i])
			size += list_size(htable->base[i]);
	return size;
}

void htable_destroy(Htable *htable) {
	for (size_t i = 0; i < htable->size; i++)
		if (NULL != htable->base[i])
			list_destroy(htable->base[i]);
	free(htable->base);
	free(htable);
}
