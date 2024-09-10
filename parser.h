#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "lexer.h"

enum BIN_OP { ADD, SUB, MUL, DIV };
enum UN_OP { NEG };

enum NODE_TYPE {
    BIN_NODE,
    UN_NODE,
    VAR_NODE,
    NUM_NODE
}; // binary node, unary node

typedef struct Node Node;

typedef struct {
    Node *lhs; // left-hand side
    Node *rhs; // right-hand side
    enum BIN_OP op;
} BinNode;

typedef struct {
    Node *child;
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
    enum NODE_TYPE type;
    union NodeData *data;
};

Node *_parse_fac(Lexer *l);
Node *_parse_term(Lexer *l);
Node *_parse_expr(Lexer *l);

Node *create_bin_node(enum BIN_OP op, Node *lhs, Node *rhs);
Node *create_un_node(enum UN_OP op, Node *n);
Node *create_var_node(const char *name);
Node *create_num_node(float num);
Node *get_parse_tree(char *expr);
void free_parse_tree(Node *n);
void print(Node *n);

#endif
