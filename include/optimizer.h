#ifndef OPTIMIZER_H_INCLUDED
#define OPTIMIZER_H_INCLUDED

#include "node_list.h"

/**
 * Optimizes a parsed tree (x=A+B), given some numerical variables (A=3, B=2).
 * If no minima exists, the function returns after reaching a MAX_STEPS
 * boundary.
 *
 * @param node Root node of the parsed tree.
 * @param numerical_vars Leaf nodes with name and value.
 * @return Minimum obtained by minimization.
 */
float optim(Node *node, NodeList *numeric_vars);

#endif
