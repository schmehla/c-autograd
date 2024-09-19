#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <stdbool.h>
#include <stdio.h>

typedef struct Node Node;

struct Node { // typedef is at the top
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

void nop(Node *this);
void eval_add(Node *this);
void eval_sub(Node *this);
void eval_mul(Node *this);
void eval_div(Node *this);
void eval_neg(Node *this);

void backprop_add(Node *this);
void backprop_sub(Node *this);
void backprop_mul(Node *this);
void backprop_div(Node *this);
void backprop_neg(Node *this);

Node *_new_node(const char *name, float val, void (*eval)(Node *this),
                void (*backprop)(Node *this), Node **children,
                size_t n_children);

Node *_new_bin_node(const char *name, void (*eval)(Node *this),
                    void (*backprop)(Node *this), Node *child_l, Node *child_r);
Node *new_num_node(float val);
Node *new_var_node(const char *name);

Node *new_add_node(const char *name, Node *child_l, Node *child_r);
Node *new_sub_node(const char *name, Node *child_l, Node *child_r);
Node *new_mul_node(const char *name, Node *child_l, Node *child_r);
Node *new_div_node(const char *name, Node *child_l, Node *child_r);
Node *new_neg_node(const char *name, Node *child);

void assign_var_node(Node *var_node, Node *child);
void unvisit(Node *root);
void set_topo_lvl(Node *root, size_t lvl);
void zero_grad(Node *root);
void full_eval(Node *root);
void full_backprop(Node *root);
void free_node(Node *node);

#endif
