#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "lexer.h"
#include "node.h"
#include "node_list.h"

NodeList *_parse_assignm_list(Lexer *l, NodeList *free_vars);
Node *_parse_assignm(Lexer *l, NodeList *free_vars);
Node *_parse_fac(Lexer *l, NodeList *free_vars);
Node *_parse_term(Lexer *l, NodeList *free_vars);
Node *_parse_expr(Lexer *l, NodeList *free_vars);

Node *parse(char *expr, NodeList *free_vars);

#endif
