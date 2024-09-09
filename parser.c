#include "parser.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

Node *parseF(Lexer *l) {
  union NodeData *nd = malloc(sizeof(union NodeData));
  Node *n = malloc(sizeof(Node));
  if (peek_token(l) == VAR) {
    advance_token(l);
    char *id = malloc((strlen(l->latest_var) + 1) * sizeof(char));
    strcpy(id, l->latest_var);
    nd->var_node.name = id;
    n->node_type = VAR_NODE;
    n->node_data = nd;
    return n;
  }
  if (peek_token(l) == NUM) {
    advance_token(l);
    nd->num_node.num = l->latest_num;
    n->node_type = NUM_NODE;
    n->node_data = nd;
    return n;
  }
  if (peek_token(l) == L_PAR) {
    advance_token(l);
    n = parseE(l);
    if (peek_token(l) != R_PAR)
      assert(false);
    advance_token(l);
    return n;
  }
  if (peek_token(l) == MINUS) {
    advance_token(l);
    nd->un_node.op = NEG;
    nd->un_node.n = parseF(l);
    n->node_type = UN_NODE;
    n->node_data = nd;
    return n;
  }
  assert(false);
}

Node *parseT(Lexer *l) {
  Node *n = parseF(l);
  if (peek_token(l) == STAR || peek_token(l) == SLASH) {
    union NodeData *nd = malloc(sizeof(union NodeData));
    Node *mul_n = malloc(sizeof(Node));
    if (peek_token(l) == STAR) {
      nd->bin_node.op = MUL;
    }
    if (peek_token(l) == SLASH)
      nd->bin_node.op = DIV;
    advance_token(l);
    nd->bin_node.lhs = n;
    nd->bin_node.rhs = parseT(l);
    mul_n->node_type = BIN_NODE;
    mul_n->node_data = nd;
    return mul_n;
  }
  return n;
}

Node *parseE(Lexer *l) {
  Node *n = parseT(l);
  while (true) {
    if (peek_token(l) == PLUS || peek_token(l) == MINUS) {
      union NodeData *nd = malloc(sizeof(union NodeData));
      Node *add_sub_n = malloc(sizeof(Node));
      if (peek_token(l) == PLUS)
        nd->bin_node.op = ADD;
      if (peek_token(l) == MINUS)
        nd->bin_node.op = SUB;
      advance_token(l);
      nd->bin_node.lhs = n;
      nd->bin_node.rhs = parseT(l);
      add_sub_n->node_type = BIN_NODE;
      add_sub_n->node_data = nd;
      n = add_sub_n;
    } else {
      return n;
    }
  }
}

void print(Node *n) {
  printf("(");
  if (n->node_type == BIN_NODE) {
    print(n->node_data->bin_node.lhs);
    if (n->node_data->bin_node.op == ADD)
      printf("+");
    if (n->node_data->bin_node.op == SUB)
      printf("-");
    if (n->node_data->bin_node.op == MUL)
      printf("*");
    if (n->node_data->bin_node.op == DIV)
      printf("/");
    print(n->node_data->bin_node.rhs);
  }
  if (n->node_type == UN_NODE) {
    if (n->node_data->un_node.op == NEG)
      printf("-");
    print(n->node_data->un_node.n);
  }
  if (n->node_type == VAR_NODE) {
    printf("%s", n->node_data->var_node.name);
  }
  if (n->node_type == NUM_NODE) {
    printf("%f", n->node_data->num_node.num);
  }
  printf(")");
}

float eval(Node *n, VarValue *var_values, size_t n_var_values) {
  if (n->node_type == BIN_NODE) {
    float lhs_eval = eval(n->node_data->bin_node.lhs, var_values, n_var_values);
    float rhs_eval = eval(n->node_data->bin_node.rhs, var_values, n_var_values);
    if (n->node_data->bin_node.op == ADD)
      return lhs_eval + rhs_eval;
    if (n->node_data->bin_node.op == SUB)
      return lhs_eval - rhs_eval;
    if (n->node_data->bin_node.op == MUL)
      return lhs_eval * rhs_eval;
    if (n->node_data->bin_node.op == DIV)
      return lhs_eval / rhs_eval;
  }
  if (n->node_type == UN_NODE) {
    float eval_ = eval(n->node_data->un_node.n, var_values, n_var_values);
    if (n->node_data->un_node.op == NEG)
      return -eval_;
  }
  if (n->node_type == VAR_NODE) {
    char *name = n->node_data->var_node.name;
    for (size_t i = 0; i < n_var_values; ++i) {
      if (strcmp(name, var_values[i].name) == 0)
        return var_values[i].value;
    }
    assert(false);
  }
  if (n->node_type == NUM_NODE) {
    return n->node_data->num_node.num;
  }
  assert(false);
}

Node *parse(Lexer *l) { return parseE(l); }
