#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "lexer.h"

enum BIN_OP { ADD, SUB, MUL, DIV };
enum UN_OP { NEG };

enum NODE_TYPE { BIN_NODE, UN_NODE, VAR_NODE, NUM_NODE };

typedef struct Node Node;

typedef struct {
  Node *lhs;
  Node *rhs;
  enum BIN_OP op;
} BinNode;

typedef struct {
  Node *n;
  enum UN_OP op;
} UnNode;

typedef struct {
  char *name;
} VarNode;

typedef struct {
  float num;
} NumNode;

union NodeData {
  BinNode bin_node;
  UnNode un_node;
  VarNode var_node;
  NumNode num_node;
};

struct Node { // typedef is at the top
  enum NODE_TYPE node_type;
  union NodeData *node_data;
};

typedef struct {
  char *name;
  float value;
} VarValue;

Node *parseF(Lexer *l);
Node *parseT(Lexer *l);
Node *parseE(Lexer *l);

Node *parse(Lexer *l);
void free_node(Node *n);

void print(Node *n);
float eval(Node *n, VarValue *var_values, size_t n_var_values);

#endif
