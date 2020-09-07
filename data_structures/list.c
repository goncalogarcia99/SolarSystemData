#include "list.h"

List_node *list_insert(List_node *list_node, void *data) {
	List_node *new_list_node = malloc(sizeof *new_list_node);
	if (NULL == new_list_node) // No memory available.
		return NULL;
	new_list_node->next = list_node;
	new_list_node->data = data;
	return new_list_node;
}

List_node *list_remove(List_node *list_node_1, List_node *list_node_2) {
	if (NULL == list_node_1 || NULL == list_node_2)
		return NULL;
	if (list_node_1 == list_node_2) {
		list_node_1 = list_node_2->next;
		free(list_node_2);
	} else {
		List_node *prev = list_node_1;
		for (List_node *current = prev->next; NULL != current; prev = current, current = current->next)
			if (current == list_node_2) {
				prev->next = list_node_2->next;
				free(list_node_2);
			}
	}
	return list_node_1;
}

List_node *list_lookup(List_node *list_node, const void *data,
							int (*cmp)(const void *, const void *, void *)) {
	for (List_node *current = list_node; NULL != current; current = current->next)
		if (!cmp(current->data, data, NULL))
			return current;
	return NULL;
}

void list_foreach(List_node *list_node, void (*func)(void *, void *), void *context) {
	for (List_node *current = list_node; NULL != current; current = current->next)
		func(current->data, context);
}

void *list_data(List_node *list_node) {
	return list_node->data;
}

size_t list_size(List_node *list_node) {
	size_t size = 0;
	for (List_node *current = list_node; NULL != current; current = current->next, size++);
	return size;
}

void list_destroy(List_node *list_node) {
	for (List_node *current = list_node, *next; NULL != current; current = next) {
		next = current->next;
		free(current);
	}
}
