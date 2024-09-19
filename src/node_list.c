#include "node_list.h"
#include "node.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

Node *pop_front(NodeList *q) {
    Node *node = NULL;
    _Elem *first = q->first;
    if (first == NULL)
        return NULL;
    node = first->data;
    q->first = first->next;
    if (q->first == NULL)
        q->last = NULL;
    free(first);
    return node;
}

void push_back(NodeList *list, Node *node) {
    _Elem *new_last = malloc(sizeof(_Elem));
    new_last->data = node;
    new_last->next = NULL;
    if (list->last)
        list->last->next = new_last;
    list->last = new_last;
    if (list->first == NULL)
        list->first = list->last;
}

Node *find_by_name(NodeList *list, char *name) {
    _Elem *curr = list->first;
    while (curr != NULL) {
        if (strcmp(name, curr->data->name) == 0) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}

NodeList *new_node_list() {
    NodeList *q = malloc(sizeof(NodeList));
    q->first = NULL;
    q->last = NULL;
    return q;
}

void free_node_list(NodeList *list) {
    _Elem *curr = list->first;
    while (curr != NULL) {
        free_node(curr->data);
        _Elem *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}
