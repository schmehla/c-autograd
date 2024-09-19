#include "tree_iter.h"
#include "node.h"
#include "node_list.h"
#include <assert.h>
#include <stdlib.h>

TreeIter *new_tree_iter(Node *node) {
    assert(node->topo_lvl != -1);
    TreeIter *it = malloc(sizeof(TreeIter));
    it->list = new_node_list();
    it->prev_lvl = 0;
    unvisit(node);
    push_back(it->list, node);
    return it;
}

void free_tree_iter(TreeIter *it) {
    free_node_list(it->list);
    free(it);
}

Node *next_in_topo(TreeIter *it) {
    while (true) {
        it->curr = pop_front(it->list);
        if (it->curr == NULL)
            return NULL;
        if (it->curr->topo_lvl <= it->prev_lvl + 1)
            break;
    }
    it->prev_lvl = it->curr->topo_lvl;
    for (size_t i = 0; i < it->curr->n_children; ++i) {
        Node *child = it->curr->children[i];
        if (child->visited)
            continue;
        child->visited = true;
        push_back(it->list, child);
    }
    return it->curr;
}

// Node *next_leaf(TreeIter *it) {
//     while (true) {
//         next_in_topo(it);
//         if (it->curr == NULL)
//             return NULL;
//         if (it->curr->n_children == 0)
//             return it->curr;
//     }
// }
