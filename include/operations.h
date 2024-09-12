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

float eval(Node *node, VarTable *var_values);
// void grad(Node *n, VarTable *var_values);

// Node *deriv(Node *n, char *by);
// Node *copy(Node *n);
void backprop(Node *node, VarTable *var_values);

#endif
