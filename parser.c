#include "parser.h"
#include "lexer.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

Node *create_bin_node(enum BIN_OP op, Node *lhs, Node *rhs) {
  Node *node = malloc(sizeof(Node));
  node->node_type = BIN_NODE;
  union NodeData *nd = malloc(sizeof(union NodeData));
  nd->bin_node.op = op;
  nd->bin_node.lhs = lhs;
  nd->bin_node.rhs = rhs;
  node->node_data = nd;
  return node;
}

Node *create_un_node(enum UN_OP op, Node *child) {
  Node *node = malloc(sizeof(Node));
  union NodeData *nd = malloc(sizeof(union NodeData));
  nd->un_node.op = NEG;
  nd->un_node.child = child;
  node->node_type = UN_NODE;
  node->node_data = nd;
  return node;
}

Node *create_var_node(const char *name) {
  Node *node = malloc(sizeof(Node));
  union NodeData *nd = malloc(sizeof(union NodeData));
  nd->var_node.name = malloc((strlen(name) + 1) * sizeof(char));
  strcpy(nd->var_node.name, name);
  node->node_type = VAR_NODE;
  node->node_data = nd;
  return node;
}

Node *create_num_node(float num) {
  Node *node = malloc(sizeof(Node));
  union NodeData *nd = malloc(sizeof(union NodeData));
  nd->num_node.num = num;
  node->node_type = NUM_NODE;
  node->node_data = nd;
  return node;
}

Node *parse_fac(Lexer *l) {
  if (peek_token(l) == L_PAR) {
    advance_token(l);
    Node *node = parse_expr(l);
    if (peek_token(l) != R_PAR)
      assert(false);
    advance_token(l);
    return node;
  }
  if (peek_token(l) == VAR) {
    advance_token(l);
    return create_var_node(l->latest_var);
  }
  if (peek_token(l) == NUM) {
    advance_token(l);
    return create_num_node(l->latest_num);
  }
  if (peek_token(l) == MINUS) {
    advance_token(l);
    return create_un_node(NEG, parse_fac(l));
  }
  assert(false);
}

Node *parse_term(Lexer *l) {
  Node *node = parse_fac(l);
  while (true) {
    if (peek_token(l) == STAR) {
      advance_token(l);
      node = create_bin_node(MUL, node, parse_fac(l));
      continue;
    }
    if (peek_token(l) == SLASH) {
      advance_token(l);
      node = create_bin_node(DIV, node, parse_fac(l));
      continue;
    }
    return node;
  }
}

Node *parse_expr(Lexer *l) {
  Node *node = parse_term(l);
  while (true) {
    if (peek_token(l) == PLUS) {
      advance_token(l);
      node = create_bin_node(ADD, node, parse_term(l));
      continue;
    }
    if (peek_token(l) == MINUS) {
      advance_token(l);
      node = create_bin_node(SUB, node, parse_term(l));
      continue;
    }
    return node;
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
    print(n->node_data->un_node.child);
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
    free_parse_tree(n->node_data->un_node.child);
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
