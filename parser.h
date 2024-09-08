#include "lexer.c"

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
  char *id;
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

Node *parseF(Lexer *l);
Node *parseT(Lexer *l);
Node *parseE(Lexer *l);
