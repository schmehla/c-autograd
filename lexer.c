#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

typedef enum { PLUS, MINUS, STAR, SLASH, L_PAR, R_PAR, VAR, NUM, EOS } Token;

typedef struct {
  const char *data;
  size_t tok_idx;
  size_t nxt_tok_idx;
  char *latest_var;
  float latest_num;
} Lexer;

char *get_var(Lexer *l) { return l->latest_var; }
float get_num(Lexer *l) { return l->latest_num; }

char nxt(Lexer *l) { return l->data[l->nxt_tok_idx]; }

bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool isalph(char c) { return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'; }

Lexer *new_lexer(const char *_data) {
  Lexer *l = malloc(sizeof(Lexer));
  l->data = _data;
  l->tok_idx = 0;
  return l;
}

void free_lexer(Lexer *l) {
  if (l->latest_var) {
    free(l->latest_var);
  }
}

void parse_num(Lexer *l) {
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
    if (!isalph(nxt(l))) {
      break;
    }
    l->nxt_tok_idx++;
  }
  size_t var_len = l->nxt_tok_idx - l->tok_idx;
  free(l->latest_var);
  l->latest_var = malloc(sizeof(char) * (var_len + 1));
  for (size_t i = 0; i < var_len; ++i) {
    l->latest_var[i] = l->data[l->tok_idx + i];
  }
  l->latest_var[var_len] = '\0';
}

// Retrieves the next token and sets the next_token index.
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
  if (isalph(nxt(l))) {
    parse_var(l);
    return VAR;
  }
  assert(false);
}

void lex(Lexer *l) { l->tok_idx = l->nxt_tok_idx; }
