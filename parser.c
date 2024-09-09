#include "parser.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

Node *create_bin_node(enum BIN_OP op, Node *lhs, Node *rhs) {
  Node *n = malloc(sizeof(Node));
  n->node_type = BIN_NODE;
  union NodeData *nd = malloc(sizeof(union NodeData));
  nd->bin_node.lhs = lhs;
  nd->bin_node.rhs = rhs;
  n->node_data = nd;
  return n;
}

Node *create_un_node(enum UN_OP op, Node *n) { return NULL; }

Node *create_var_node(const char *name) { return NULL; }

Node *create_num_node(float num) { return NULL; }

Node *parse_fac(Lexer *l) {
  if (peek_token(l) == L_PAR) {
    advance_token(l);
    Node *n = parse_expr(l);
    if (peek_token(l) != R_PAR)
      assert(false);
    advance_token(l);
    return n;
  }
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
  if (peek_token(l) == MINUS) {
    advance_token(l);
    nd->un_node.op = NEG;
    nd->un_node.n = parse_fac(l);
    n->node_type = UN_NODE;
    n->node_data = nd;
    return n;
  }
  assert(false);
}

Node *parse_term(Lexer *l) {
  Node *n = parse_fac(l);
  while (true) {
    if (peek_token(l) == STAR || peek_token(l) == SLASH) {
      union NodeData *nd = malloc(sizeof(union NodeData));
      Node *mul_div_n = malloc(sizeof(Node));
      if (peek_token(l) == STAR)
        nd->bin_node.op = MUL;
      // Node *mul = create_bin_node(MUL, n, parse_fac(l));
      if (peek_token(l) == SLASH)
        nd->bin_node.op = DIV;
      advance_token(l);
      nd->bin_node.lhs = n;
      nd->bin_node.rhs = parse_fac(l);
      mul_div_n->node_type = BIN_NODE;
      mul_div_n->node_data = nd;
      n = mul_div_n;
    } else {
      return n;
    }
  }
}

Node *parse_expr(Lexer *l) {
  Node *n = parse_term(l);
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
      nd->bin_node.rhs = parse_term(l);
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

Node *get_parse_tree(Lexer *l) { return parse_expr(l); }

void free_parse_tree(Node *n) {
  if (n->node_type == BIN_NODE) {
    free_parse_tree(n->node_data->bin_node.lhs);
    free_parse_tree(n->node_data->bin_node.rhs);
  }
  if (n->node_type == UN_NODE) {
    free_parse_tree(n->node_data->un_node.n);
  }
  if (n->node_type == VAR_NODE) {
    free(n->node_data->var_node.name);
  }
  if (n->node_type == NUM_NODE) {
    // do nothing
  }
  free(n->node_data);
  free(n);
}
