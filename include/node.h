#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

typedef struct Node Node;

struct Node {
    char *name;
    float val;
    void (*eval)(Node *this);
    float grad;
    void (*backprop)(Node *this);
    Node **children;
    size_t n_children;
    size_t topo_lvl;
    bool visited;
};

/**
 * Contructs a new addition / subtraction / multiplication / divison node.
 *
 * @param name Name of the node.
 * @param child_l Left child.
 * @param child_r Right child.
 * @return New node.
 */
Node *new_add_node(const char *name, Node *child_l, Node *child_r);
Node *new_sub_node(const char *name, Node *child_l, Node *child_r);
Node *new_mul_node(const char *name, Node *child_l, Node *child_r);
Node *new_div_node(const char *name, Node *child_l, Node *child_r);

/**
 * Contructs a new negation node.
 *
 * @param name Name of the node.
 * @param child Child.
 * @return New node.
 */
Node *new_neg_node(const char *name, Node *child);

/**
 * Contructs a new numeric node. This is just a number in the expression. It has
 * no name and no children.
 *
 * @return New node.
 */
Node *new_num_node(float val);

/**
 * Contructs a new variable node. This is just a named leaf node with a value
 * coming from the variable assignments. Has no children.
 *
 * @param name Name of the node.
 * @return New node.
 */
Node *new_var_node(const char *name);

/**
 * A `visited` flag for every node exists for a topological iterator. Before
 * the iterator runs, it has to clean up this flag. This happens recusively.
 *
 * @param root Root node of the tree to clean.
 */
void unvisit(Node *root);

/**
 * This function should has to be called before any backprop call. Could be
 * moved into the backprop function but is called on every run then (not
 * needed). A `topo_lvl` number for every node exists for a topological
 * iterator. It needs a topology level to properly find the next node.
 *
 * @param root Root node of the tree to clean.
 * @param lvl Topology level to set on root.
 */
void set_topo_lvl(Node *root, size_t lvl);

/**
 *
 *
 * @param root
 */
void zero_grad(Node *root);

/**
 *
 *
 * @param root
 */
void eval(Node *root);

/**
 *
 *
 * @param root
 */
void backprop(Node *root);

/**
 *
 *
 * @param root
 */
void free_node(Node *root);

/**
 *
 *
 * @param node
 */
void free_numeric_var_node(Node *node);

#endif
