#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    PLUS,
    MINUS,
    STAR,
    SLASH,
    L_PAR,
    R_PAR,
    VAR,
    NUM,
    NEWLINE,
    EQUALS,
    EOS
} Token;

typedef struct {
    char *expr;
    size_t tok_idx;
    size_t nxt_tok_idx;
    char *latest_var;
    float latest_num;
} Lexer;

void _parse_num(Lexer *l);
void _parse_var(Lexer *l);

Lexer *new_lexer(const char *_data);
void free_lexer(Lexer *l);
Token peek_token(Lexer *l);
void scan_token(Lexer *l);

#endif
