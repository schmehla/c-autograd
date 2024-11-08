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

/**
 * Pops the first element of the list.
 *
 * @param list List to operator on.
 * @return Popped node.
 */
Node *pop_front(NodeList *list);

/**
 * Pushed to the end of the list.
 *
 * @param list List to operator on.
 * @param node Node to append.
 */
void push_back(NodeList *list, Node *node);

/**
 * Find a node by name.
 *
 * @param list List to operator on.
 * @param name Name of the node to find
 * @return Node with given name.
 */
Node *find_by_name(NodeList *list, char *name);

/**
 * Create a new node list.
 *
 * @return Node list.
 */
NodeList *new_node_list();

/**
 * Free the node list.
 *
 * @param list List to operator on.
 */
void free_node_list(NodeList *list);

#endif
