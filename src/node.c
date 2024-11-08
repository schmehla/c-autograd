#include "node.h"
#include "tree_iter.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Empty operation, used for leaf nodes.
 *
 * @private
 * @param this Node to operate on. Resembles OOP.
 */
void _nop(Node *this) {}

/**
 * Calculate node value from adding / subtracting / multiplying / dividing /
 * negating children values.
 *
 * @private
 * @param this Node to operate on. Resembles OOP.
 */
void _eval_add(Node *this) {
    this->val = this->children[0]->val + this->children[1]->val;
}
void _eval_sub(Node *this) {
    this->val = this->children[0]->val - this->children[1]->val;
}
void _eval_mul(Node *this) {
    this->val = this->children[0]->val * this->children[1]->val;
}
void _eval_div(Node *this) {
    this->val = this->children[0]->val / this->children[1]->val; // div by zero?
}
void _eval_neg(Node *this) { this->val = -this->children[0]->val; }

/**
 * Calculate children node gradients from current node gradients and values.
 * Mathematically: d(x□y)/dx, □ is from {+,-,*,/} for one child x
 *
 * @private
 * @param this Node to operate on. Resembles OOP.
 */
void _backprop_add(Node *this) {
    this->children[0]->grad += this->grad;
    this->children[1]->grad += this->grad;
}
void _backprop_sub(Node *this) {
    this->children[0]->grad += this->grad;
    this->children[1]->grad -= this->grad;
}
void _backprop_mul(Node *this) {
    float l_val = this->children[0]->val;
    float r_val = this->children[1]->val;
    this->children[0]->grad += r_val * this->grad;
    this->children[1]->grad += l_val * this->grad;
}
void _backprop_div(Node *this) {
    float l_val = this->children[0]->val;
    float r_val = this->children[1]->val;
    this->children[0]->grad += (1 / r_val) * this->grad;
    this->children[1]->grad -= (l_val / (r_val * r_val)) * this->grad;
}
void _backprop_neg(Node *this) { this->children[0]->grad -= this->grad; }

/**
 * Contructs a new node.
 *
 * @private
 * @param name Name of the node.
 * @param val Initial value of the node.
 * @param eval Function pointer to the nodes eval function. Resembles
               polymorphism to prevent switch cases over the node type in every
               function operating on nodes.
 * @param backprop Function pointer to the backprop function. Like eval.
 * @param children Array of children pointers.
 * @param n_children Length of children array.
 * @return New node.
 */
Node *_new_node(const char *name, float val, void (*eval)(Node *this),
                void (*backprop)(Node *this), Node **children,
                size_t n_children) {
    Node *node = malloc(sizeof(Node));
    if (name == NULL) {
        node->name = NULL;
    } else {
        node->name = malloc(sizeof(char) * (strlen(name) + 1));
        strcpy(node->name, name);
    }
    node->val = val;
    node->children = children;
    node->n_children = n_children;
    node->eval = eval;
    node->visited = false;
    node->topo_lvl = -1;
    node->backprop = backprop;
    return node;
}

/**
 * Contructs a new node with binary operations, as we have many of these ops.
 * This is just for a further wrapper for caller convenience.
 *
 * @private
 * @param name Name of the node.
 * @param eval Function pointer to the nodes eval function.
 * @param backprop Function pointer to the backprop function.
 * @param child_l Left child.
 * @param child_r Right child.
 * @return New node.
 */
Node *_new_bin_node(const char *name, void (*eval)(Node *this),
                    void (*backprop)(Node *this), Node *child_l,
                    Node *child_r) {
    Node **children = malloc(2 * sizeof(Node *));
    children[0] = child_l;
    children[1] = child_r;
    Node *node = _new_node(name, 0, eval, backprop, children, 2);
    return node;
}

/*******************************************************************************
 * PUBLIC API
 ******************************************************************************/

Node *new_add_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, _eval_add, _backprop_add, child_l, child_r);
}
Node *new_sub_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, _eval_sub, _backprop_sub, child_l, child_r);
}
Node *new_mul_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, _eval_mul, _backprop_mul, child_l, child_r);
}
Node *new_div_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, _eval_div, _backprop_div, child_l, child_r);
}

Node *new_neg_node(const char *name, Node *child) {
    Node **children = malloc(sizeof(Node *));
    children[0] = child;
    Node *node = _new_node(name, 0, _eval_neg, _backprop_neg, children, 1);
    return node;
}

Node *new_num_node(float val) {
    return _new_node(NULL, val, _nop, _nop, NULL, 0);
}

Node *new_var_node(const char *name) {
    return _new_node(name, 0, _nop, _nop, NULL, 0);
}

void unvisit(Node *node) {
    node->visited = false;
    for (size_t i = 0; i < node->n_children; ++i)
        unvisit(node->children[i]);
}

void set_topo_lvl(Node *node, size_t lvl) {
    if (lvl < node->topo_lvl) {
        node->topo_lvl = lvl;
    }
    for (size_t i = 0; i < node->n_children; ++i)
        set_topo_lvl(node->children[i], lvl + 1);
}

void zero_grad(Node *node) {
    node->grad = 0;
    for (size_t i = 0; i < node->n_children; ++i)
        zero_grad(node->children[i]);
}

void eval(Node *node) {
    for (size_t i = 0; i < node->n_children; ++i)
        eval(node->children[i]);
    node->eval(node);
}

void backprop(Node *root) {
    root->grad = 1;
    TreeIter *it = new_tree_iter(root);
    Node *curr = root;
    while ((curr = next_in_topo(it))) {
        curr->backprop(curr);
    }
    free_tree_iter(it);
}

void free_node(Node *node) {
    if (node->n_children == 0 && node->name != NULL)
        return;
    for (size_t i = 0; i < node->n_children; ++i)
        free_node(node->children[i]);
    free(node->name);
    free(node->children);
    free(node);
}

void free_numeric_var_node(Node *node) {
    free(node->name);
    free(node->children);
    free(node);
}
