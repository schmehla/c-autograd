#include "parser.h"
#include "lexer.h"
#include "node.h"
#include "node_list.h"
#include <assert.h>

NodeList *_parse_assignm_list(Lexer *l, NodeList *free_vars) {
    NodeList *assignments = new_node_list();
    while (true) {
        push_back(assignments, _parse_assignm(l, free_vars));
        if (peek_token(l) != NEWLINE || peek_token(l) == EOS) {
            assert(false);
        }
        scan_token(l);
    }
    // assert(free_vars->first == NULL && free_vars->last == NULL);
}

Node *_parse_assignm(Lexer *l, NodeList *free_vars) {
    if (peek_token(l) != VAR)
        assert(false);
    scan_token(l);
    Node *var_node = new_var_node(l->latest_var);
    if (peek_token(l) != EQUALS)
        assert(false);
    Node *expr_node = _parse_expr(l, free_vars);
    assign_var_node(var_node, expr_node);
    return var_node;
}

Node *_parse_fac(Lexer *l, NodeList *free_vars) {
    if (peek_token(l) == L_PAR) {
        scan_token(l);
        Node *node = _parse_expr(l, free_vars);
        if (peek_token(l) != R_PAR)
            assert(false);
        scan_token(l);
        return node;
    }
    if (peek_token(l) == VAR) {
        scan_token(l);
        Node *var_node = find_by_name(free_vars, l->latest_var);
        if (var_node == NULL) {
            var_node = new_var_node(l->latest_var);
            push_back(free_vars, var_node);
        }
        return var_node;
    }
    if (peek_token(l) == NUM) {
        scan_token(l);
        return new_num_node(l->latest_num);
    }
    if (peek_token(l) == MINUS) {
        scan_token(l);
        return new_neg_node(NULL, _parse_fac(l, free_vars));
    }
    assert(false);
}

Node *_parse_term(Lexer *l, NodeList *free_vars) {
    Node *node = _parse_fac(l, free_vars);
    while (true) {
        if (peek_token(l) == STAR) {
            scan_token(l);
            node = new_mul_node(NULL, node, _parse_fac(l, free_vars));
            continue;
        }
        if (peek_token(l) == SLASH) {
            scan_token(l);
            node = new_div_node(NULL, node, _parse_fac(l, free_vars));
            continue;
        }
        return node;
    }
}

Node *_parse_expr(Lexer *l, NodeList *free_vars) {
    Node *node = _parse_term(l, free_vars);
    while (true) {
        if (peek_token(l) == PLUS) {
            scan_token(l);
            node = new_add_node(NULL, node, _parse_term(l, free_vars));
            continue;
        }
        if (peek_token(l) == MINUS) {
            scan_token(l);
            node = new_sub_node(NULL, node, _parse_term(l, free_vars));
            continue;
        }
        return node;
    }
}

Node *parse(char *expr, NodeList *free_vars) {
    Lexer *l = new_lexer(expr);
    Node *parse_tree = _parse_expr(l, free_vars);
    free_lexer(l);
    return parse_tree;
}
