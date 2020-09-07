#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct List_node {
	struct List_node *next;
	void *data;
} List_node;

List_node *list_insert(List_node *list_node, void *data);
List_node *list_remove(List_node *list_node1, List_node *list_node2);
List_node *list_lookup(List_node *list_node, const void *key, int (*cmp)(const void *, const void *, void *));
void list_foreach(List_node *list_node, void (*func)(void *, void *), void *context);
void *list_data(List_node *list_node);
size_t list_size(List_node *list_node);
void list_destroy(List_node *list_node);

#endif
