#include "lexer.c"
// #include "ast.c"
#include <stdio.h>

int main(void) {
  // struct Expr *e = new_expr(MUL, new_val(0.1f), new_val(0.2f));
  // print_expr(e);
  // printf("\n");
  // printf("eval: %f", eval(e));

  char expr_str[] = "(aaa 1.1 * 0.2 /    10 +   hAllo)";
  Lexer *l = new_lexer(expr_str);
  Token next = peek_token(l);
  lex(l);
  while (next != EOS) {
    if (next == PLUS) {
      printf("+");
    }
    if (next == MINUS) {
      printf("-");
    }
    if (next == STAR) {
      printf("*");
    }
    if (next == SLASH) {
      printf("/");
    }
    if (next == L_PAR) {
      printf("(");
    }
    if (next == R_PAR) {
      printf(")");
    }
    if (next == VAR) {
      printf("%s", get_var(l));
    }
    if (next == NUM) {
      printf("%f", get_num(l));
    }
    printf("\n");
    next = next_token(l);
    lex(l);
  }

  // free_expr(e);
  free_lexer(l);
  printf("\n");
  return 0;
}
