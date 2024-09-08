#include "parser.h"
#include <string.h>

Node *parseF(Lexer *l) {
  // if (t == EOS) return ;
  union NodeData *nd = malloc(sizeof(union NodeData));
  Node *n = malloc(sizeof(Node));
  if (peek_token(l) == VAR) {
    lex(l);
    char *id = malloc((strlen(l->latest_var) + 1) * sizeof(char));
    strcpy(id, l->latest_var);
    nd->var_node.id = id;
    n->node_type = VAR_NODE;
    n->node_data = nd;
    return n;
  }
  if (peek_token(l) == NUM) {
    lex(l);
    nd->num_node.num = l->latest_num;
    n->node_type = NUM_NODE;
    n->node_data = nd;
    return n;
  }
  if (peek_token(l) == L_PAR) {
    lex(l);
    n = parseE(l);
    if (peek_token(l) != R_PAR)
      assert(false);
    lex(l);
    return n;
  }
  if (peek_token(l) == NEG) {
    nd->un_node.op = NEG;
    n->node_type = UN_NODE;
    n->node_data = nd;
    return n;
  }
  assert(false);
}

Node *parseT(Lexer *l) {
  Node *n = parseF(l);
  if (peek_token(l) == STAR || peek_token(l) == SLASH) {
    lex(l);
    union NodeData *nd = malloc(sizeof(union NodeData));
    Node *mul_n = malloc(sizeof(Node));
    nd->bin_node.lhs = n;
    nd->bin_node.rhs = parseT(l);
    if (peek_token(l) == STAR)
      nd->bin_node.op = MUL;
    if (peek_token(l) == SLASH)
      nd->bin_node.op = DIV;
    mul_n->node_type = BIN_NODE;
    mul_n->node_data = nd;
    return n;
  }
  return n;
}

Node *parseE(Lexer *l) {
  Node *n = parseT(l);
  while (true) {
    if (peek_token(l) == PLUS) {
      return n;
    }
    if (peek_token(l) == PLUS) {
      return n;
    }
  }
  return n;
}
