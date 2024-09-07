#include <stdlib.h>

enum TOKEN { L_PAR, R_PAR, OP, VAL };

struct {
  enum TOKEN t;
  char c;
} lookup[] = {{.t = L_PAR, .c = '('}, {.t = R_PAR, .c = ')'}, {}, {}};

struct Lexer {
  char *data;
  char *curr;
  float latest_val;
  enum BIN_OP latest_bin_op;
};

float get_latest_val(struct Lexer *l) { return l->latest_val; }

struct Lexer *new_lexer(char *_data) {
  struct Lexer *l = malloc(sizeof(struct Lexer));
  l->data = _data;
  return l;
}

enum TOKEN lex(struct Lexer *l) {

};
