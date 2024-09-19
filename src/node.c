#include "node.h"
#include "tree_iter.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void nop(Node *this) {}
void eval_add(Node *this) {
    this->val = this->children[0]->val + this->children[1]->val;
}
void eval_sub(Node *this) {
    this->val = this->children[0]->val - this->children[1]->val;
}
void eval_mul(Node *this) {
    this->val = this->children[0]->val * this->children[1]->val;
}
void eval_div(Node *this) {
    this->val = this->children[0]->val / this->children[1]->val; // div by zero
}
void eval_neg(Node *this) { this->val = -this->children[0]->val; }

void backprop_add(Node *this) {
    this->children[0]->grad += this->grad;
    this->children[1]->grad += this->grad;
}
void backprop_sub(Node *this) {
    this->children[0]->grad += this->grad;
    this->children[1]->grad -= this->grad;
}
void backprop_mul(Node *this) {
    float l_val = this->children[0]->val;
    float r_val = this->children[1]->val;
    this->children[0]->grad += r_val * this->grad;
    this->children[1]->grad += l_val * this->grad;
}
void backprop_div(Node *this) {
    float l_val = this->children[0]->val;
    float r_val = this->children[1]->val;
    this->children[0]->grad += (1 / r_val) * this->grad;
    this->children[1]->grad -= (l_val / (r_val * r_val)) * this->grad;
}
void backprop_neg(Node *this) { this->children[0]->grad -= this->grad; }

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

Node *_new_bin_node(const char *name, void (*eval)(Node *this),
                    void (*backprop)(Node *this), Node *child_l,
                    Node *child_r) {
    Node **children = malloc(2 * sizeof(Node *));
    children[0] = child_l;
    children[1] = child_r;
    Node *node = _new_node(name, 0, eval, backprop, children, 2);
    return node;
}

Node *new_add_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, eval_add, backprop_add, child_l, child_r);
}
Node *new_sub_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, eval_sub, backprop_sub, child_l, child_r);
}
Node *new_mul_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, eval_mul, backprop_mul, child_l, child_r);
}
Node *new_div_node(const char *name, Node *child_l, Node *child_r) {
    return _new_bin_node(name, eval_div, backprop_div, child_l, child_r);
}

Node *new_neg_node(const char *name, Node *child) {
    Node **children = malloc(sizeof(Node *));
    children[0] = child;
    Node *node = _new_node(name, 0, eval_neg, backprop_neg, children, 1);
    return node;
}

Node *new_num_node(float val) {
    return _new_node(NULL, val, nop, nop, NULL, 0);
}

Node *new_var_node(const char *name) {
    return _new_node(name, 0, nop, nop, NULL, 0);
}

void assign_var_node(Node *var_node, Node *child) {
    var_node->n_children = 1;
    Node **children = malloc(sizeof(Node *));
    children[0] = child;
    var_node->children = children;
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

void full_eval(Node *node) {
    for (size_t i = 0; i < node->n_children; ++i)
        full_eval(node->children[i]);
    node->eval(node);
}

void full_backprop(Node *root) {
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
