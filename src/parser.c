#include "parser.h"
#include "lexer.h"
#include "node.h"
#include "node_list.h"
#include <assert.h>

/*
 * The actual formal grammar for the parser comes from this YouTube video:
 * https://www.youtube.com/watch?v=SToUyjAsaFk
 * Watching it helps massively to understand this code.
 * There are non-terminals called factors (F), terms (T) and expressions (E).
 * Variables (Var) and numbers (Num) are terminal.
 * Production rules:
 *   F -> Var | Num | E | -F
 *   T → F * T | F / T | F
 *   E → T + E | T - E | T
 */

// Forward declaration for cyclically dependent functions.
Node *_parse_term(Lexer *l, NodeList *numeric_vars);
Node *_parse_expr(Lexer *l, NodeList *numeric_vars);

/**
 * Parses a factor (like in 2 * x, 2 and x is a factor). This can be either be a
 * variable, a number, an expression (see _parse_expr) or a negated factor.
 * The production rule is: F -> Var | Num | E | -F
 * If a production is applied that produces a non-terminal, further recursive
 * calls follow.
 *
 * @private
 * @param l Lexer instance.
 * @param numeric_vars List of nodes that only have a (numeric) value and no
                       children.
 * @return A node with child nodes if recursive calls happened.
 */
Node *_parse_fac(Lexer *l, NodeList *numeric_vars) {
    Node *node;
    switch (peek_token(l)) {
    case L_PAR:
        scan_token(l);
        node = _parse_expr(l, numeric_vars);
        if (peek_token(l) != R_PAR)
            assert(false);
        scan_token(l);
        break;
    case VAR:
        scan_token(l);
        node = find_by_name(numeric_vars, l->latest_var);
        if (node == NULL) {
            node = new_var_node(l->latest_var);
            push_back(numeric_vars, node);
        }
        break;
    case NUM:
        scan_token(l);
        node = new_num_node(l->latest_num);
        break;
    case MINUS:
        scan_token(l);
        node = new_neg_node(NULL, _parse_fac(l, numeric_vars));
        break;
    default:
        assert(false); // TODO replace all assertions with errors
        break;
    }
    return node;
}

/**
 * Parses a term (like in 2 * [..], [..] is a term).
 * The production rule is: T → F * T | F / T | F
 * If a production is applied that produces a non-terminal, further recursive
 * calls follow. The node = new..(.., node..) call replaces the node with a new
 * node that is put on "top" of it. This ensures proper associativity, as the
 * resulting tree implies left-brackets.
 *
 * @private
 * @param l Lexer instance.
 * @param numeric_vars List of nodes that only have a (numeric) value and no
                       children.
 * @return A node with child nodes if recursive calls happened.
 */
Node *_parse_term(Lexer *l, NodeList *numeric_vars) {
    Node *node = _parse_fac(l, numeric_vars);
    for (Token tok = peek_token(l); tok == STAR || tok == SLASH;
         tok = peek_token(l)) {
        scan_token(l);
        switch (tok) {
        case STAR:
            node = new_mul_node(NULL, node, _parse_term(l, numeric_vars));
            break;
        case SLASH:
            node = new_div_node(NULL, node, _parse_term(l, numeric_vars));
            break;
        default:
            assert(false);
            break;
        }
    }
    return node;
}

/**
 * Parses an expression (like in 2 + 4, 2 is the expression).
 * The production rule is: E → T + E | T - E | T
 * If a production is applied that produces a non-terminal, further recursive
 * calls follow. The node = new..(.., node..) call replaces the node with a new
 * node that is put on "top" of it. This ensures proper associativity, as the
 * resulting tree implies left-brackets.
 *
 * @private
 * @param l Lexer instance.
 * @param numeric_vars List of nodes that only have a (numeric) value and no
                       children.
 * @return A node with child nodes if recursive calls happened.
 */
Node *_parse_expr(Lexer *l, NodeList *numeric_vars) {
    Node *node = _parse_term(l, numeric_vars);
    for (Token tok = peek_token(l); tok == PLUS || tok == MINUS;
         tok = peek_token(l)) {
        scan_token(l);
        switch (tok) {
        case PLUS:
            node = new_add_node(NULL, node, _parse_term(l, numeric_vars));
            break;
        case MINUS:
            node = new_sub_node(NULL, node, _parse_term(l, numeric_vars));
            break;
        default:
            assert(false);
            break;
        }
    }
    return node;
}

/*******************************************************************************
 * PUBLIC API
 ******************************************************************************/

Node *parse(char *expr, NodeList *numeric_vars) {
    Lexer *l = new_lexer(expr);
    Node *parse_tree = _parse_expr(l, numeric_vars);
    free_lexer(l);
    return parse_tree;
}
