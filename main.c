#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

enum BIN_OP { ADD, SUB, MUL, DIV };

enum EXPR_TYPE { VAL, EXPR };

struct Expr {
  enum EXPR_TYPE type;
  union {
    float val;
    struct {
      enum BIN_OP bin_op;
      struct Expr *lhs;
      struct Expr *rhs;
    };
  } u;
};

struct Expr *new_val(float _val) {
  struct Expr *e = malloc(sizeof(struct Expr));
  e->type = VAL;
  e->u.val = _val;
  return e;
}

struct Expr *new_expr(enum BIN_OP _bin_op, struct Expr *_lhs,
                      struct Expr *_rhs) {
  struct Expr *e = malloc(sizeof(struct Expr));
  e->type = EXPR;
  e->u.bin_op = _bin_op;
  e->u.lhs = _lhs;
  e->u.rhs = _rhs;
  return e;
}

void free_expr(struct Expr *e) {
  if (e->type == VAL) {
    free(e);
  } else if (e->type == EXPR) {
    free_expr(e->u.lhs);
    free_expr(e->u.rhs);
  }
}

float eval(struct Expr *e) {
  if (e->type == VAL) {
    return e->u.val;
  } else if (e->type == EXPR) {
    float lhs_eval = eval(e->u.lhs);
    float rhs_eval = eval(e->u.rhs);
    switch (e->u.bin_op) {
    case ADD:
      return lhs_eval + rhs_eval;
    case SUB:
      return lhs_eval - rhs_eval;
    case MUL:
      return lhs_eval * rhs_eval;
    case DIV:
      return lhs_eval / rhs_eval;
    }
  }
  assert(0);
}

void print_expr(struct Expr *e) {
  if (e->type == VAL) {
    printf("%f", e->u.val);
  } else {
    printf("(");
    print_expr(e->u.lhs);
    switch (e->u.bin_op) {
    case ADD:
      printf(" + ");
      break;
    case SUB:
      printf(" - ");
      break;
    case MUL:
      printf(" * ");
      break;
    case DIV:
      printf(" / ");
      break;
    }
    print_expr(e->u.rhs);
    printf(")");
  }
}

int main(void) {
  // struct Expr *e = new_expr(MUL, new_val(0.1f), new_val(0.2f));
  // print_expr(e);
  // printf("\n");
  // printf("eval: %f", eval(e));

  char expr_str[] = "(0.1 * 0.2)";
  char *curr = &expr_str[0];
  while (*curr != '\0') {
    switch (*curr) {
    case ' ':
      break;
    case '(':

      break;
    }
    curr++;
  }

  // free_expr(e);
  printf("\n");
  return 0;
}
