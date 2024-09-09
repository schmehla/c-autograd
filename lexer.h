#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

typedef enum { PLUS, MINUS, STAR, SLASH, L_PAR, R_PAR, VAR, NUM, EOS } Token;

typedef struct {
  const char *data;
  size_t tok_idx;
  size_t nxt_tok_idx;
  char *latest_var;
  float latest_num;
} Lexer;

Lexer *new_lexer(const char *_data);
void free_lexer(Lexer *l);
Token peek_token(Lexer *l);
void advance_token(Lexer *l);

#endif
