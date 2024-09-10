#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include "parser.h"

typedef struct {
    char *name;
    float value;
} VarValue;

typedef struct {
    VarValue *arr;
    size_t len;
} VarValues;

float eval(Node *n, VarValues *var_values);
// void simpl(Node *n, Node *parent);
Node *deriv(Node *n, char *by);
Node *copy(Node *n);

#endif
