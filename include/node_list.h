#ifndef NODE_LIST_H_INCLUDED
#define NODE_LIST_H_INCLUDED

#include "node.h"

typedef struct _Elem _Elem;
struct _Elem {
    _Elem *next;
    Node *data;
};

typedef struct {
    _Elem *first;
    _Elem *last;
} NodeList;

Node *pop_front(NodeList *list);
void push_back(NodeList *list, Node *node);
Node *find_by_name(NodeList *list, char *name);
NodeList *new_node_list();
void free_node_list(NodeList *list);

#endif
