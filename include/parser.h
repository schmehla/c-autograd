#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "lexer.h"
#include "node.h"

typedef struct {
    Node *root;
    NodeList **vars;
} ParseTree;

Node *_parse_fac(Lexer *l, NodeList **vars);
Node *_parse_term(Lexer *l, NodeList **vars);
Node *_parse_expr(Lexer *l, NodeList **vars);

ParseTree *get_parse_tree(char *expr);
void free_parse_tree(ParseTree *pt);

#endif
