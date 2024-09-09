#include "lexer.h"
#include "parser.h"
#include <stdio.h>

int main(void) {
  // struct Expr *e = new_expr(MUL, new_val(0.1f), new_val(0.2f));
  // print_expr(e);
  // printf("\n");
  // printf("eval: %f", eval(e));

  // char expr_str[] = "(1.1 - (-2/2) * (-0.2) / x*x + 101 - 101)";
  char expr_str[] = "(1 / 2 / 5)";
  Lexer *l = new_lexer(expr_str);
  Node *n = parse(l);
  print(n);
  VarValue var_values[] = {{.name = "x", .value = 10}};
  float e = eval(n, var_values, 1);
  printf("\nevals to: %f", e);
  // Token next = peek_token(l);
  // lex(l);
  // while (next != EOS) {
  //   if (next == PLUS) {
  //     printf("+");
  //   }
  //   if (next == MINUS) {
  //     printf("-");
  //   }
  //   if (next == STAR) {
  //     printf("*");
  //   }
  //   if (next == SLASH) {
  //     printf("/");
  //   }
  //   if (next == L_PAR) {
  //     printf("(");
  //   }
  //   if (next == R_PAR) {
  //     printf(")");
  //   }
  //   if (next == VAR) {
  //     printf("%s", get_var(l));
  //   }
  //   if (next == NUM) {
  //     printf("%f", get_num(l));
  //   }
  //   printf("\n");
  //   next = peek_token(l);
  //   lex(l);
  // }

  // free_expr(e);
  free_lexer(l);
  printf("\n");
  return 0;
}
