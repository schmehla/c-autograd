#include "lexer.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

// Convenience functions.
char _nxt(Lexer *l) { return l->expr[l->nxt_tok_idx]; }
bool _is_d(char c) { return isdigit((unsigned char)c); }
bool _is_a(char c) { return isalpha((unsigned char)c); }

/**
 * Parses a number matching `\d+(\.\d+)?`.
 *
 * @private
 * @param l Lexer instance.
 */
void _parse_num(Lexer *l) {
    float num = 0;
    while (_nxt(l) != '\0' && _is_d(_nxt(l))) {
        num *= 10;
        num += (_nxt(l) - '0');
        l->nxt_tok_idx++;
    }
    if (_nxt(l) != '\0' && _nxt(l) == '.') {
        float d = 10;
        l->nxt_tok_idx++;
        while (_nxt(l) != '\0' && _is_d(_nxt(l))) {
            num += (_nxt(l) - '0') / d;
            d *= 10;
            l->nxt_tok_idx++;
        }
    }
    l->latest_num = num;
}

/**
 * Parses a variable name matching `[a-zA-Z]+`.
 *
 * @private
 * @param l Lexer instance.
 */
void _parse_var(Lexer *l) {
    size_t max_var_len = 20; // TODO make dynamic length
    for (size_t i = 0; i < max_var_len; ++i) {
        if (!_is_a(_nxt(l))) {
            break;
        }
        l->nxt_tok_idx++;
    }
    size_t var_len = l->nxt_tok_idx - l->tok_idx;
    free(l->latest_var);
    l->latest_var = malloc((var_len + 1) * sizeof(char));
    strncpy(l->latest_var, l->expr + l->tok_idx, var_len);
    l->latest_var[var_len] = '\0';
}

/*******************************************************************************
 * PUBLIC API
 ******************************************************************************/

Lexer *new_lexer(const char *expr) {
    Lexer *l = malloc(sizeof(Lexer));
    l->expr = malloc(strlen(expr) + 1);
    strcpy(l->expr, expr);
    l->tok_idx = 0;
    l->latest_var = NULL;
    return l;
}

void free_lexer(Lexer *l) {
    if (l->latest_var) {
        free(l->latest_var);
    }
    free(l->expr);
    free(l);
}

Token peek_token(Lexer *l) {
    // Prevent overflowing and skip whitespaces.
    if (l->expr[l->tok_idx] == '\0') {
        return EOS;
    }
    while (l->expr[l->tok_idx] == ' ') {
        l->tok_idx++;
    }
    // Now find the correct next token index.
    l->nxt_tok_idx = l->tok_idx;
    switch (_nxt(l)) {
    case '+':
        l->nxt_tok_idx++;
        return PLUS;
    case '-':
        l->nxt_tok_idx++;
        return MINUS;
    case '*':
        l->nxt_tok_idx++;
        return STAR;
    case '/':
        l->nxt_tok_idx++;
        return SLASH;
    case '(':
        l->nxt_tok_idx++;
        return L_PAR;
    case ')':
        l->nxt_tok_idx++;
        return R_PAR;
    default:
        if (_is_d(_nxt(l))) {
            _parse_num(l);
            return NUM;
        }
        if (_is_a(_nxt(l))) {
            _parse_var(l);
            return VAR;
        }
    }
    assert(false);
}

void scan_token(Lexer *l) { l->tok_idx = l->nxt_tok_idx; }
