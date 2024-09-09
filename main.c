#include "lexer.h"
#include "operations.h"
#include "parser.h"
#include <stdio.h>

int main(void) {
  // struct Expr *e = new_expr(MUL, new_val(0.1f), new_val(0.2f));
  // print_expr(e);
  // printf("\n");
  // printf("eval: %f", eval(e));

  // char expr_str[] = "(2 * 3 / (1.1 - (-2/2) * (-0.2) / x*x + 101 - 101)) --
  // 2";
  char expr_str[] = "(1 * 2 * x * x)";
  Lexer *lexer = new_lexer(expr_str);
  Node *parse_tree = get_parse_tree(lexer);

  print(parse_tree);

  VarValue var_values[] = {{.name = "x", .value = 10}};
  float e = eval(parse_tree, var_values, 1);
  printf("\nevals to: %f\n", e);

  Node *d_parse_tree = deriv(parse_tree);
  print(d_parse_tree);

  e = eval(d_parse_tree, var_values, 1);
  printf("\nevals to: %f\n", e);

  free_lexer(lexer);
  free_parse_tree(parse_tree);
  free_parse_tree(d_parse_tree);
  printf("\n");
  return 0;
}
