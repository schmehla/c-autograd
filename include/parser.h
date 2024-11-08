#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "node.h"
#include "node_list.h"

/**
 * Returns a parse tree instance for the expression.
 *
 * @param expr String containing the expression to parse.
 * @param numeric_vars List of nodes that only have a (numeric) value and no
                       children. These are represented by e.g. A = 2, B = 1
 * @return New lexer instance.
 */
Node *parse(char *expr, NodeList *numeric_vars);

#endif
