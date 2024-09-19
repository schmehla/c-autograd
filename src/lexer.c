#include "lexer.h"

#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

char _nxt(Lexer *l) { return l->expr[l->nxt_tok_idx]; }

bool _is_d(char c) { return isdigit((unsigned char)c); }
bool _is_a(char c) { return isalpha((unsigned char)c); }

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

void _parse_num(Lexer *l) { // TODO no lib reimpl
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

void _parse_var(Lexer *l) {
    size_t max_var_len = 20; // TODO bah
    for (size_t i = 0; i < max_var_len; ++i) {
        if (!_is_a(_nxt(l))) {
            break;
        }
        l->nxt_tok_idx++;
    }
    size_t var_len = l->nxt_tok_idx - l->tok_idx;
    free(l->latest_var);
    l->latest_var = malloc(sizeof(char) * (var_len + 1));
    for (size_t i = 0; i < var_len; ++i) {
        l->latest_var[i] = l->expr[l->tok_idx + i]; // TODO no lib reimpl
    }
    l->latest_var[var_len] = '\0';
}

// Retrieves the next token and sets the nxt_tok_idx.
// Needs to be called before lex() at least once.
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
    if (_nxt(l) == '+') {
        l->nxt_tok_idx++;
        return PLUS;
    }
    if (_nxt(l) == '-') {
        l->nxt_tok_idx++;
        return MINUS;
    }
    if (_nxt(l) == '*') {
        l->nxt_tok_idx++;
        return STAR;
    }
    if (_nxt(l) == '/') {
        l->nxt_tok_idx++;
        return SLASH;
    }
    if (_nxt(l) == '(') {
        l->nxt_tok_idx++;
        return L_PAR;
    }
    if (_nxt(l) == ')') {
        l->nxt_tok_idx++;
        return R_PAR;
    }
    if (_nxt(l) == '\n') {
        l->nxt_tok_idx++;
        return NEWLINE;
    }
    if (_nxt(l) == '=') {
        l->nxt_tok_idx++;
        return EQUALS;
    }
    if (_is_d(_nxt(l))) {
        _parse_num(l);
        return NUM;
    }
    if (_is_a(_nxt(l))) {
        _parse_var(l);
        return VAR;
    }
    assert(false);
}

void scan_token(Lexer *l) { l->tok_idx = l->nxt_tok_idx; }
