#ifndef TREE_ITER_H_INCLUDED
#define TREE_ITER_H_INCLUDED

#include "node.h"
#include "node_list.h"

typedef struct {
    NodeList *list;
    Node *curr;
    size_t prev_lvl;
} TreeIter;

TreeIter *new_tree_iter(Node *node);
void free_tree_iter(TreeIter *it);
Node *next_in_topo(TreeIter *it);

#endif
