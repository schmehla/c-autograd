#include "lexer.h"

#include <assert.h>
#include <sys/types.h>

char nxt(Lexer *l) { return l->data[l->nxt_tok_idx]; }

bool is_digit(char c) { return c >= '0' && c <= '9'; } // TODO no lib reimpl
bool is_alpha(char c) {
  return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z';
} // TODO no lib reimpl

Lexer *new_lexer(const char *_data) {
  Lexer *l = malloc(sizeof(Lexer));
  l->data = _data;
  l->tok_idx = 0;
  l->latest_var = NULL;
  return l;
}

void free_lexer(Lexer *l) {
  if (l->latest_var) {
    free(l->latest_var);
  }
}

void parse_num(Lexer *l) { // TODO no lib reimpl
  float num = 0;
  while (nxt(l) != '\0' && is_digit(nxt(l))) {
    num *= 10;
    num += (nxt(l) - '0');
    l->nxt_tok_idx++;
  }
  if (nxt(l) != '\0' && nxt(l) == '.') {
    float d = 10;
    l->nxt_tok_idx++;
    while (nxt(l) != '\0' && is_digit(nxt(l))) {
      num += (nxt(l) - '0') / d;
      d *= 10;
      l->nxt_tok_idx++;
    }
  }
  l->latest_num = num;
}

void parse_var(Lexer *l) {
  size_t max_var_len = 20;
  char var[max_var_len];
  for (size_t i = 0; i < max_var_len; ++i) {
    if (!is_alpha(nxt(l))) {
      break;
    }
    l->nxt_tok_idx++;
  }
  size_t var_len = l->nxt_tok_idx - l->tok_idx;
  free(l->latest_var);
  l->latest_var = malloc(sizeof(char) * (var_len + 1));
  for (size_t i = 0; i < var_len; ++i) {
    l->latest_var[i] = l->data[l->tok_idx + i]; // TODO no lib reimpl
  }
  l->latest_var[var_len] = '\0';
}

// Retrieves the next token and sets the nxt_tok_idx.
// Needs to be called before lex() at least once.
Token peek_token(Lexer *l) {
  // Prevent overflowing and skip whitespaces.
  if (l->data[l->tok_idx] == '\0') {
    return EOS;
  }
  while (l->data[l->tok_idx] == ' ') {
    l->tok_idx++;
  }
  // Now find the correct next token index.
  l->nxt_tok_idx = l->tok_idx;
  if (nxt(l) == '+') {
    l->nxt_tok_idx++;
    return PLUS;
  }
  if (nxt(l) == '-') {
    l->nxt_tok_idx++;
    return MINUS;
  }
  if (nxt(l) == '*') {
    l->nxt_tok_idx++;
    return STAR;
  }
  if (nxt(l) == '/') {
    l->nxt_tok_idx++;
    return SLASH;
  }
  if (nxt(l) == '(') {
    l->nxt_tok_idx++;
    return L_PAR;
  }
  if (nxt(l) == ')') {
    l->nxt_tok_idx++;
    return R_PAR;
  }
  if (is_digit(nxt(l))) {
    parse_num(l);
    return NUM;
  }
  if (is_alpha(nxt(l))) {
    parse_var(l);
    return VAR;
  }
  assert(false);
}

void advance_token(Lexer *l) { l->tok_idx = l->nxt_tok_idx; }
