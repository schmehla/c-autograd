#include "parser.h"
#include "lexer.h"
#include "node.h"
#include <assert.h>

Node *_parse_fac(Lexer *l, NodeList **vars) {
    if (peek_token(l) == L_PAR) {
        advance_token(l);
        Node *node = _parse_expr(l, vars);
        if (peek_token(l) != R_PAR)
            assert(false);
        advance_token(l);
        return node;
    }
    if (peek_token(l) == VAR) {
        advance_token(l);
        return create_var_node(l->latest_var, vars);
    }
    if (peek_token(l) == NUM) {
        advance_token(l);
        return create_num_node(l->latest_num, vars);
    }
    if (peek_token(l) == MINUS) {
        advance_token(l);
        return create_un_node(NEG, _parse_fac(l, vars), vars);
    }
    assert(false);
}

Node *_parse_term(Lexer *l, NodeList **vars) {
    Node *node = _parse_fac(l, vars);
    while (true) {
        if (peek_token(l) == STAR) {
            advance_token(l);
            node = create_bin_node(MUL, node, _parse_fac(l, vars), vars);
            continue;
        }
        if (peek_token(l) == SLASH) {
            advance_token(l);
            node = create_bin_node(DIV, node, _parse_fac(l, vars), vars);
            continue;
        }
        return node;
    }
}

Node *_parse_expr(Lexer *l, NodeList **vars) {
    Node *node = _parse_term(l, vars);
    while (true) {
        if (peek_token(l) == PLUS) {
            advance_token(l);
            node = create_bin_node(ADD, node, _parse_term(l, vars), vars);
            continue;
        }
        if (peek_token(l) == MINUS) {
            advance_token(l);
            node = create_bin_node(SUB, node, _parse_term(l, vars), vars);
            continue;
        }
        return node;
    }
}

ParseTree *get_parse_tree(char *expr) {
    Lexer *l = new_lexer(expr);
    NodeList **vars = empty_node_list();
    Node *root = _parse_expr(l, vars);
    free_lexer(l);
    ParseTree *parse_tree = malloc(sizeof(ParseTree));
    parse_tree->root = root;
    parse_tree->vars = vars;
    return parse_tree;
}

void free_parse_tree(ParseTree *pt) {
    free_node(pt->root);
    free_node_list(pt->vars);
    free(pt);
}
