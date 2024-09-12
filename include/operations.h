#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include "node.h"
#include <stddef.h>

typedef struct {
    char *name;
    float value;
} Var;

typedef struct {
    Var *arr;
    size_t len;
} VarTable;

void eval(Node *node, VarTable *var_table);
// void grad(Node *n, VarTable *var_values);

// Node *deriv(Node *n, char *by);
// Node *copy(Node *n);
void _backprop(Node *node);
void backprop(Node *node);

#endif
