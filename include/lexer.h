#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

typedef enum { PLUS, MINUS, STAR, SLASH, L_PAR, R_PAR, VAR, NUM, EOS } Token;

typedef struct {
    char *expr;
    size_t tok_idx;
    size_t nxt_tok_idx;
    char *latest_var;
    float latest_num;
} Lexer;

/**
 * Returns new lexer instance.
 *
 * @param expr String containing the expression to create a lexer on.
 * @return New lexer instance.
 */
Lexer *new_lexer(const char *expr);

/**
 * Frees lexer.
 *
 * @param l Lexer instance.
 */
void free_lexer(Lexer *l);

/**
 * Looks at the next token but do not scan it (1-lookahead). Retrieves the next
 * token and sets the nxt_tok_idx that's used by scan_token. Therefore, it needs
 * to be called before scan_token() at least once. Multiple calls are allowed.
 *
 * @param l Lexer instance.
 * @return Peeked token.
 */
Token peek_token(Lexer *l);

/**
 * Consume the current token. Changes lexer state.
 *
 * @param l Lexer instance.
 */
void scan_token(Lexer *l);

#endif
