#ifndef OPERATIONS_H_INCLUDED
#define OPERATIONS_H_INCLUDED

#include "parser.h"

float eval(Node *n, VarValue *var_values, size_t n_var_values);

Node *derivative(Node *n);
Node *copy(Node *n);

#endif
