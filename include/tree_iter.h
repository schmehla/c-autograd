#ifndef TREE_ITER_H_INCLUDED
#define TREE_ITER_H_INCLUDED

#include "node.h"
#include "node_list.h"

typedef struct {
    NodeList *list;
    Node *curr;
    size_t prev_lvl;
} TreeIter;

/**
 * Creates new tree iterator.
 *
 * @param root Root node of the tree.
 * @return Iterator created for the tree.
 */
TreeIter *new_tree_iter(Node *root);

/**
 * Frees a tree iterator.
 *
 * @param it Iterator to free.
 */
void free_tree_iter(TreeIter *it);

/**
 * Returns the next node in the tree given by a topological sorting.
 *
 * @param it Iterator to work on.
 * @return Next node in tolopoly.
 */
Node *next_in_topo(TreeIter *it);

#endif
